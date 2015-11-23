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
[RequireComponent(typeof(RoboAction))]
[RequireComponent(typeof(_Utilities))]
public class FirstAvoidanceProtocol : MonoBehaviour {

    #region Builds

    RoboAction Actions;
    Network_Protocols _network;
    _Utilities util;

    #endregion Builds

    #region Prefs

    #region CONFIG Look in the Unity Inspector
    public int PING_TOO_CLOSE = 90;
    public int PING_PROCEED = 100;

    public int REAR_PING_TOO_CLOSE = 40;

    public int FORWARD_TIME_MS = 3000; // how long to drive before turning (milliseconds)
    public int SEARCH_TIME_MS = 2000; // how long to search before reversing (milliseconds)
    public int REVERSE_TIME_MS = 1000; // max time to reverse
    public float TURN_TIME = 1.5f; // time to turn after a reverse or moving for a while (in seconds)

    public int ACCEL_X_ZERO_MIN = -75;
    public int ACCEL_X_ZERO_MAX = 75;

    public int ACCEL_Y_ZERO_MIN = -100;
    public int ACCEL_Y_ZERO_MAX = 100;

    public int PING_AVG_RESOLUTION = 100; // how many pings to use for average
    #endregion CONFIG

    #region Private_Access

    #region BRAINS variables forwhich one thinks of oneself
    bool searching = false;
    bool reversing = false;
    bool turnRight = false;
    long moveStart = 0;
    long turnStart = 0;
    #endregion BRAINS

    #region AVERAGES internal raw data tracked for averaging purposes
    Queue<int> pingQueue = new Queue<int>();
    Queue<int> rearPingQueue = new Queue<int>();
    #endregion AVERAGES

    #region HELD_variables

    System.Random random;
    int ping;  //{ get; set; } // moving average
    int pingRear;// { get; private set;  } // moving average
    #endregion HELD_variables

    #endregion Private_Access

    #endregion Prefs
    
    #region Startup
    void Start() {
        Actions = GetComponent<RoboAction>();
        _network = GetComponent<Network_Protocols>();
        util = GetComponent<_Utilities>();
        random = new System.Random();
        ping = util.ping;
        pingRear = util.pingRear;
        // start it up...
        //util.StartCoroutine(util.ParseNetwork());
    }
    #endregion Startup

    #region Ping_Averaging
    //public int ping;  //{ get; set; } // moving average

    public void setPing (int ping)
    {
        if (ping>0)
        {
            /*
            // compute standard deviation (NOT USED YET)
            int[] pings = pingQueue.ToArray();
            double avg = pings.Average();
            double sumOfSquaresOfDifferences = pings.Select(val => (ping - avg) * (ping - avg)).Sum();
            double sd = Math.Sqrt(sumOfSquaresOfDifferences / pings.Length);
            */

            // queue up to configured resolution
            pingQueue.Enqueue(ping);
            while (pingQueue.Count > PING_AVG_RESOLUTION)
            {
                pingQueue.Dequeue();
                //pingQueue.TrimToSize();
            }

            // compute average and assign
            int sum = 0;
            foreach (int raw in pingQueue.ToArray())
            {
                sum += raw;
            }
            this.ping = sum / pingQueue.Count; // integer division (remainder dropped)
        }
    }

    //public int pingRear;// { get; private set;  } // moving average

    public void setRearPing(int ping)
    {
        if (ping > 0)
        {
            /*
            // compute standard deviation (NOT USED YET)
            int[] pings = pingQueue.ToArray();
            double avg = pings.Average();
            double sumOfSquaresOfDifferences = pings.Select(val => (ping - avg) * (ping - avg)).Sum();
            double sd = Math.Sqrt(sumOfSquaresOfDifferences / pings.Length);
            */

            // queue up to configured resolution
            rearPingQueue.Enqueue(ping);
            while (rearPingQueue.Count > PING_AVG_RESOLUTION)
            {
                rearPingQueue.Dequeue();
                //pingQueue.TrimToSize();
            }

            // compute average and assign
            int sum = 0;
            foreach (int raw in rearPingQueue.ToArray())
            {
                sum += raw;
            }
            this.pingRear = sum / rearPingQueue.Count; // integer division (remainder dropped)
        }
    }

    #endregion Ping_Averaging

    #region DEMOMODE this is DEMOAVOIDANCE mode
    /**
    * AutoMobile is the avoidance routine brains.
    * Watch out for zombies!
    */
    public IEnumerator AutoMobile() {
        _network.Stop();
        moveStart = DateTime.Now.Ticks / TimeSpan.TicksPerMillisecond;

        while (true) {
            long now = DateTime.Now.Ticks / TimeSpan.TicksPerMillisecond;

            if (searching) {
                // too close!
                if (ping >= 0 && ping < PING_TOO_CLOSE)  {
                    if (reversing)  {
                        // ready to stop reversing?
                        if ((pingRear > 0 && pingRear < REAR_PING_TOO_CLOSE) || now - moveStart >= REVERSE_TIME_MS)   {
                            reversing = false;
                            turnRight = (random.Next(0, 2) == 0);
                            if (turnRight) _network.Right();
                            else _network.Left();
                            turnStart = now + (long)(TURN_TIME * 1000); ;
                            yield return new WaitForSeconds(TURN_TIME);
                        }
                        else {     // back it up!
                            _network.Reverse();
                        }
                    }
                    else  {  // normal turn
                        if (now - turnStart >= SEARCH_TIME_MS) { // turn times out?
                            reversing = true;
                            moveStart = DateTime.Now.Ticks / TimeSpan.TicksPerMillisecond;
                        }
                        else // spin right round!
                        {
                            if (turnRight) _network.Right();
                            else _network.Left();
                        }
                    }
                }
                else // all is clear -- done searching
                {
                    _network.Stop();
                    searching = false;
                    moveStart = now;
                }
            }
            else // going (not searching)
            {
                // too close!
                if (ping > 0 && ping < PING_TOO_CLOSE) 
                {
                    _network.Stop();
                    searching = true;
                    reversing = false;
                    turnStart = DateTime.Now.Ticks / TimeSpan.TicksPerMillisecond;
                    turnRight = (random.Next(0, 2) == 0);
                }
                // tired of going this way?
                else if (now - moveStart >= FORWARD_TIME_MS) 
                {
                    turnRight = (random.Next(0, 2) == 0);
                    if (turnRight) _network.Right();
                    else _network.Left();
                    moveStart = now + (long)(TURN_TIME * 1000);
                    yield return new WaitForSeconds(TURN_TIME);
                }
                else // proceed
                { 
                    _network.Forward();
                }
            }

            yield return null;
        }
    }

    #endregion DEMOMODE

    // Communicate -- not in use
    void TalkBack(int Left, int Right) {
        GetComponent<Network_Protocols>().DriveL = Left;
        GetComponent<Network_Protocols>().DriveR = Right;
    }

}
