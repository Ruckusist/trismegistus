using UnityEngine;
using System.Collections;
using UnityEngine.UI;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System;

public class NetworkSend : MonoBehaviour
{
    /** the instance of the robot (car) that Unity and comms will interact with */
    public static NetworkSend instance; // for access to instance data by other scripts
    // UI
    // prefs
    public string IncomingIP = ""; // is now dynamic
    public string IP = "192.168.2.172"; //default local
    public int port = 7799; // define > init
    // gui
    //string strMessage = "";
    public Text ErrorControl;
    public Text Status;
    public Text RawDataStream;
    //public string message = "";

    IPEndPoint remoteEndPoint;
    UdpClient client;

    public void Awake()
    {
        //remoteEndPoint = new IPEndPoint(IPAddress.Parse(IP), port);

        //string IncomingIP = Robot.instance.INCOMING;
        remoteEndPoint = new IPEndPoint(IPAddress.Parse(Robot.instance.INCOMING), port); // this works
        //Status.text = "Sending: " + message;
        Status.text = "Sending...";
    }

    public void Start()
    {
        //instance = this;
        StartCoroutine(MyCoroutine());
    }

    // Update is called once per frame
    IEnumerator MyCoroutine()
    {
        client = new UdpClient(port);                // this is the call to port... needs to be right...
        while (true)
        {
            try
            {
                //byte[] data = Encoding.UTF8.GetBytes(message);
                byte[] data = Encoding.UTF8.GetBytes(Robot.instance.driveL.ToString() + "," + Robot.instance.driveR.ToString()
                    + ",L::" + Robot.instance.driveL.ToString() + "  R::" + Robot.instance.driveR.ToString());
                client.Send(data, data.Length, remoteEndPoint);
            }
            catch (Exception err)
            {
                print(err.ToString());
                ErrorControl.text = err.ToString();
            }
            Status.text = "Connected to: /n" + IncomingIP;

            //yield return new WaitForSeconds(0.003f);
            yield return null;
        }
    }
    /*
    public void SetMessage(string message)
    {
        this.message = message;
    }
    */
}
