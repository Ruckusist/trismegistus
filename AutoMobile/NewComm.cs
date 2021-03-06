using UnityEngine;
using System.Collections;
using UnityEngine.UI;
using System.Net.Sockets;
using System.Net;
using System.Text;
using System;

public class NewComm : MonoBehaviour
{

    // CONFIGURATION
    public const int localUDPPort = 9977;
    public const int remoteUDPPort = 7799;

    // UI
    public Text status;                         // General As-Is Status
    public Text statusRemoteIP;                 // Exerpt of data - Incoming IP
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

    public int[] speed = { 200, 200 };


    // INTERNAL STUFF
    UdpClient receiver;                         // Unity Built in UDP API stuff... 
    UdpClient sender;                           // Unity Built in UDP API stuff... 
    IPEndPoint remoteIp;                        // Unity Built in UDP API stuff...  for send back

    //This Should run as a CoRoutine or as a Thread... more research needs doing 
    // this is the Looped per framerate function
    void Update()
    {
        if (receiver != null)
        {     // If everything is working... keep going

            try
            {                   // try { more notes}; 
                IPEndPoint anyIP = new IPEndPoint(IPAddress.Any, 0);
                byte[] data = receiver.Receive(ref anyIP);

                string received = Encoding.UTF8.GetString(data);

                RawDataStream.text = "Raw Data Stream::: " + received;    // This is the imediate output of everything for debuging
                handleUDPData(received);                                  // This is the call to parse that data 

                // This is the motor controls that we are sending... already brilliantly setup as a public... just needs slider attached.. make slider...
                byte[] toSend = Encoding.UTF8.GetBytes(speed[0].ToString() + "," + speed[1].ToString());
                sender.Send(toSend, toSend.Length, remoteIp);             // this is obviously failing... but its right here... the problem... the answer will go here...
            }
            catch (Exception err)
            {
                ErrorControl.text = err.ToString();                         // this shouldnt happen... but when it does... its a huge long 3 line error... and is super helpful...
            }
        }
    }

    public void BeginEverything()
    {
        status.text = "opening UDP port";

        receiver = new UdpClient(localUDPPort);
        sender = new UdpClient();

        status.text = "running";
    }

    private void handleUDPData(string udp)
    {
        if (udp == null || udp.Length < 1) { return; }  // this never happens...

        String[] data = udp.Split(',');                 // this can be changed to better arange the data ... but ... this is fine...

        if (data.Length > 2)                              // yeah... so the data is obviously longer than 2... 
        {
            remoteIp = new IPEndPoint(IPAddress.Parse(data[0]), remoteUDPPort);

            statusRemoteIP.text = "car IP: " + data[0];
            display1.text = "ping: " + data[1];
            display2.text = "audio: " + data[2];
            display3.text = "Accel of x ::: " + data[3];
            display4.text = "Accel of y ::: " + data[4];
            display5.text = "Accel of z ::: " + data[5];
            display6.text = "Gyro of x ::: " + data[6];
            display7.text = "Gyro of y ::: " + data[7];
            display8.text = "Gyro of z ::: " + data[8];
        }
    }
}
