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
using System.Collections.Generic;
using System;
#endregion using

[RequireComponent(typeof(Network_Protocols))]
public class _Utilities : MonoBehaviour {
    // OH YEAH fucking utilities...

    #region Builds
    Network_Protocols _network;
    #endregion Builds
    
    #region PUBLIC_Access
    public int
        PingF,
        PingR,
        AccelX,
        AccelY,
        AccelZ,
        GyroX,
        GyroY,
        GyroZ,
        PING_AVG_RESOLUTION = 1;

    public int ping { get; set; }              // moving average
    public int pingRear { get; private set;  } // moving average

    #endregion PUBLIC_Access

    #region PRIVATE_Access
    private float
        _accelX,
        _accelY,
        _accelZ,
        _gyroX,
        _gyroY,
        _gyroZ;

    public Vector3 AccelVector;
    public Vector3 GyroVector;
    enum accelAxis {
        Normal = 0,
        Switched,
        MAX
    }

    Vector3[] horizontalAxes = new Vector3[(int)accelAxis.MAX];
    Vector3[] verticalAxes = new Vector3[(int)accelAxis.MAX];

    accelAxis currentAxis = accelAxis.Normal;

    #region AVERAGES internal raw data tracked for averaging purposes
    Queue<int> pingQueue = new Queue<int>();
    Queue<int> rearPingQueue = new Queue<int>();
    #endregion AVERAGES

    #endregion PRIVATE_Access

    void Start() {
        _network = GetComponent<Network_Protocols>();
    }

    public IEnumerator ParseNetwork() {
        while (true) {
            try {
                //get data
                string _data = GetComponent<Network_Protocols>().IncomingMessage;
                //split data
                string[] data = _data.Split(',');                   // this can be changed to better arange the data ... but ... this is fine...
                // parse data
                if (data.Length >= 10) {
                    // Inputs
                    AccelX = int.Parse(data[3]);
                    _accelX = AccelX;
                    AccelY = int.Parse(data[4]);
                    _accelY = AccelY;
                    AccelZ = int.Parse(data[5]);
                    _accelZ = AccelZ;
                    GyroX = int.Parse(data[6]);
                    _gyroX = GyroX;
                    GyroY = int.Parse(data[7]);
                    _gyroY = GyroY;
                    GyroZ = int.Parse(data[8]);
                    _gyroZ = GyroZ;
                    AccelVector = new Vector3(_accelX, _accelY, _accelZ);
                    GyroVector = new Vector3(_gyroX, _gyroY, _gyroZ);
                    PingF = int.Parse(data[2]);
                    setPing(PingF);
                    PingR = int.Parse(data[9]);
                    pingRear = PingR;
                    //setRearPing(PingR);
                }
            }
            catch (Exception err) { print(err.ToString()); }
            yield return null;
        }
    }

    
    #region MOVINGAVERAGE

    void setPing(int ping) {
        if (ping > 0) {
            pingQueue.Enqueue(ping);
            while (pingQueue.Count > PING_AVG_RESOLUTION) {
                pingQueue.Dequeue();
            }
            // compute average and assign
            int sum = 0;
            foreach (int raw in pingQueue.ToArray()) {
                sum += raw;
            }
            this.ping = sum / pingQueue.Count; // integer division (remainder dropped)
        }
    }
    
    void setRearPing(int ping) {
        

        //if (ping > 0) {
        //    rearPingQueue.Enqueue(ping);
        //    while (rearPingQueue.Count > PING_AVG_RESOLUTION) {
        //        rearPingQueue.Dequeue();
        //    }
        //    // compute average and assign
        //    int sum2 = 0;
        //    foreach (int raw2 in rearPingQueue.ToArray()) {
        //        sum2 += raw2;
        //    }
        //    this.pingRear = sum2 / rearPingQueue.Count; // integer division (remainder dropped)
        //}
    }

    #endregion MOVINGAVERAGE

    #region Operational_AXES

    public IEnumerator FlyAccelGyro() {   // this is so not workings
        ParseNetwork();
        while (true) {
            try {
                horizontalAxes[0] = new Vector3(_accelX, 0f, 0f);  // setup input on the 1 positive
                horizontalAxes[1] = new Vector3(0f, 0f,_accelZ);   // setup input on the 1 negitive

                 verticalAxes[0] = new Vector3(0f, _accelY, 0f);
                 verticalAxes[1] = new Vector3(_accelX, 0f, 0f);
            }
            catch (Exception err) { print(err.ToString()); }
            yield return null;
        }
    }

    public void motion(Rigidbody rb, float speed) {
        // get input
        float moveHorizontal = Input.GetAxis("accelX");
        float moveVeritical = Input.GetAxis("accelY");

        // set input on appropriate axes
        Vector3 horizontalMovement = horizontalAxes[(int)currentAxis] * moveHorizontal;
        Vector3 verticalMovement = verticalAxes[(int)currentAxis] * moveVeritical;

        // get total movement
        Vector3 movement = horizontalMovement + verticalMovement;

        rb.AddForce(movement * speed);
    }

    #endregion Operational_AXES
}
