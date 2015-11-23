#region INCLUDES Libraries
using UnityEngine;
using System.Collections;
using System;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using UnityEngine.UI;
#endregion INCLUDES


//[RequireComponent (typeof(????))]
public class NetworkRec : MonoBehaviour {

    #region PREFS
    #region UI PULBIC UI ELEMENTS
    public Text status;                         // General As-Is Status
    public Text status2;                        // INcoming IP address
    public Text ErrorControl;                   // All Errors in One Place
    public Text RawDataStream;                  // DeBug Output stream of Data right off the tap
    public Text display1;                       // Data 1 - dB level
    public Text display2;                       // Data 2 - ping sensor
    public Text display3;                       // Data 3 - ??
    public Text display4;                       // Data 4
    public Text display5;                       // Data 5
    public Text display6;                       // Data 6
    public Text display7;                       // Data 7
    public Text display8;                       // Data 8
    public Text display9;                       // TOO MUCH DATAS MUGHGHAHAHAHAH!!!
    #endregion UI

    #region Vars Class Variables
    //public string IP = "192.168.2.177"; //default local
    public int port = 9977; // define > init
    #endregion Vars

    #region setup Elements
    // udpclient object
    UdpClient client;
    #endregion setup

    #endregion PREFS

    #region STARTUP crap
    void Awake() {
        status.text = "Disconnected";
    }

    
    public void Start() {
        StartCoroutine(MyCoroutine());
    }
    #endregion STARTUP

    #region NETWORKREC

    IEnumerator MyCoroutine() {
        port = 9977;     // this should be dynamic someday...
        client = new UdpClient(port);                // this is the call to port... needs to be right...

        while (true) {
            try {
                IPEndPoint anyIP = new IPEndPoint(IPAddress.Any, 0);
                byte[] data = client.Receive(ref anyIP);
                string received = Encoding.UTF8.GetString(data);
                RawDataStream.text = "Raw Data Stream::: " + received;    // This is the imediate output of everything for debuging
                FillTelemetryPanel(received);
            }
            catch (Exception err) {
                print(err.ToString());
                ErrorControl.text = err.ToString();
            }
            status.text = "Connected";
            yield return null;
        }
    }

    private void FillTelemetryPanel(string udp) {
        #region IFTHERESDATA then do something
        if (udp == null || udp.Length < 1) { return; }  // this never happens...

        String[] data = udp.Split(',');                 // this can be changed to better arange the data ... but ... this is fine...

        if (data.Length >= 10) {
            #endregion IFTHERESDATA

            #region SETROBOT elements for processing
            Robot.instance.setPing(int.Parse(data[2]));
            Robot.instance.setRearPing(int.Parse(data[9]));
            Robot.instance.accelX = int.Parse(data[6]);
            Robot.instance.accelY = int.Parse(data[7]);
            Robot.instance.accelZ = int.Parse(data[8]);
            Robot.instance.INCOMING = data[0];
            #endregion SETROBOT

            #region SETDISPLAY of Telemetry
            display1.text = "ping: " + Robot.instance.ping;
            display2.text = "audio: " + data[1];
            display3.text = "Accel of x ::: " + data[3];
            display4.text = "Accel of y ::: " + data[4];
            display5.text = "Accel of z ::: " + data[5];
            display6.text = "Gyro of x ::: " + data[6];
            display7.text = "Gyro of y ::: " + data[7];
            display8.text = "Gyro of z ::: " + data[8];
            display9.text = "rear ping: " + Robot.instance.pingRear;
            #endregion SETDISPLAY
        }
    }

    #endregion NETWORKREC
}