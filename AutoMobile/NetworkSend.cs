using UnityEngine;
using System.Collections;
using UnityEngine.UI;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System;

public class NetworkSend : MonoBehaviour {

    // UI
    // prefs
    public string IP = "192.168.2.172"; //default local
    public int port = 7799; // define > init
    // gui
    //string strMessage = "";
    public Text ErrorControl;
    public Text Status;
    public Text RawDataStream;
    public string message = "666,-666";

    IPEndPoint remoteEndPoint;
    UdpClient client;

    public void Awake() {
        remoteEndPoint = new IPEndPoint(IPAddress.Parse(IP), port);
        Status.text = "Disconnected";
    }

    public void Start() {
        StartCoroutine(MyCoroutine());
    }

    // Update is called once per frame
    IEnumerator MyCoroutine() {
        client = new UdpClient(port);                // this is the call to port... needs to be right...
        while (true) {
            try {
                byte[] data = Encoding.UTF8.GetBytes(message);
                client.Send(data, data.Length, remoteEndPoint);
            }
            catch (Exception err) {
                print(err.ToString());
                ErrorControl.text = err.ToString();
            }
            Status.text = "Connected";
            yield return null;
        }
    }
}
