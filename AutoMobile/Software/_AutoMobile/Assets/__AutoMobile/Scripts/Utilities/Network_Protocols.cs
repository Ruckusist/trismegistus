#region Copywrite
// AutoMobile library collection
// Oct-2015 by Eric Petersen <Ruckusist@outlook.com>
// Updates should always be available at https://github.com/Ruckusist/trismegistus
//
// Changelog:
//     ... - ongoing debug release

// NOTE: THIS IS ONLY A PARIAL RELEASE. THIS DEVICE CLASS IS CURRENTLY UNDERGOING ACTIVE
// DEVELOPMENT AND IS STILL MISSING SOME IMPORTANT FEATURES. PLEASE KEEP THIS IN MIND IF
// YOU DECIDE TO USE THIS PARTICULAR CODE FOR ANYTHING.

/* ============================================
AutoMobile  library code is placed under the MIT license
Copyright (c) 2015 Eric Petersen

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
===============================================
*/
#endregion Copywrite

#region using
using UnityEngine;
using System.Collections;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System;
#endregion using

public class Network_Protocols : MonoBehaviour {

    #region Builds

    public static Network_Protocols instance; // for access to instance data by other scripts
    UdpClient recClient;
    UdpClient sendClient;

    #endregion Builds

    #region Prefs
    
    #region Send_Prefs
    //public string IncomingIP;          // dynamically set for Send if !null then !HasSendTarget
    IPAddress incoming_IP;
    protected int sendPort = 7799;     // Arbitrary but neccessary

    public int DriveL = 0;             // Main Left drive Motor Conrol
    public int DriveR = 0;             // Main Right drive Motor Control
    public string NodeSelect = "7";
    public string MegaSelect = "1";
    public bool HasSendTarget;
    #endregion Send_Prefs

    #region Rec_Prefs
    protected int recPort = 9977;          // this is made up but needs to be same on the Car
    public string IncomingMessage;         // Incoming Data for Parse In other script
    #endregion Rec_Prefs

    #endregion Prefs
    
    #region DRIVEMODE Wheel Control Available to all Scripts
    public void Stop() {
        DriveL = 0;
        DriveR = 0;
    }
    public void Forward() {
        DriveL = 250;
        DriveR = 250;
    }
    public void Reverse() {
        DriveL = -250;
        DriveR = -250;
    }
    public void Left() {
        DriveL = -250;
        DriveR = 250;
    }
    public void Right() {
        DriveL = 250;
        DriveR = -250;
    }
    #endregion DRIVEMODE

    public IEnumerator NetRecRoutine() {
        recClient = new UdpClient(recPort);                // this is the call to port... needs to be right...
        while (true) {
            try {
                IPEndPoint anyIP = new IPEndPoint(IPAddress.Any, 0);
                byte[] data = recClient.Receive(ref anyIP);
                incoming_IP = anyIP.Address;
                string received = Encoding.UTF8.GetString(data);
                IncomingMessage = received;
            }
            catch (Exception err) { print(err.ToString()); }
            yield return null;
        }
    }

    public IEnumerator SendRoutine() {
        sendClient = new UdpClient(sendPort);                // this is the call to port... needs to be right...
        while (true) {
            try {
                string _elementsplit = ",";
                string _element1 = DriveL.ToString();
                string _element2 = DriveR.ToString();
                string _element3 = ",L::" + DriveL + "  R::" + DriveR;
                byte[] data = Encoding.UTF8.GetBytes(NodeSelect +
                    _elementsplit +
                    MegaSelect +
                    _elementsplit +
                    _element1 +
                    _elementsplit +
                    _element2 +
                    _elementsplit +
                    _element3);  // this could be a for loop          

                IPEndPoint remoteEndPoint = new IPEndPoint(incoming_IP, sendPort);
                sendClient.Send(data, data.Length, remoteEndPoint);
            }
            catch (Exception err) { print(err.ToString()); }
            yield return null;
        }
    }
}
