using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using System;
using System.Collections.Generic;
using System.Linq;

[RequireComponent(typeof(RobotFollow))]
[RequireComponent(typeof(NavMeshAgent))]
public class Robot : MonoBehaviour {
    /*             --------- NOTES ---------
     * ~ We should be started when Unity begins
     * ~ the instance of the robot (car) that Unity and comms will interact with */

    #region PREFS
    public static Robot instance; // for access to instance data by other scripts
    System.Random random;

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

    #region TELEMETRY incoming used data streams


    public int ping { get; set; } // moving average

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

    public int pingRear { get; private set;  } // moving average

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

    public int accelX;
    public int accelY;
    public int accelZ;
    public string INCOMING;
    #endregion TELEMETRY

    #region OUTGOING motordrive data sent out to "Kit"
    public int driveL = 0;
    public int driveR = 0;
    #endregion OUTGOING

    #region GAMEPLAYER variables for Game Character Robot
    public int speed = 2;
    #endregion GAMEPLAYER

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
    #endregion PREFS

    #region STARTUP crap
    public void Start()
    {
        instance = this;
        random = new System.Random();
    }
    #endregion STARTUP

    
    public void Update() { // On screen action per frame update
        // backwards
        if (accelX < ACCEL_X_ZERO_MIN) {
            transform.Translate(0, 0, -speed * Time.deltaTime);
        }
        // forwards
        else if (accelX > ACCEL_X_ZERO_MAX) {
            transform.Translate(0, 0, speed * Time.deltaTime);
        }
        // left
        if (accelY < ACCEL_Y_ZERO_MIN) {
            transform.Translate(speed * Time.deltaTime, 0, 0);
        }
        // right
        else if (accelY > ACCEL_Y_ZERO_MAX) {
            transform.Translate(-speed * Time.deltaTime, 0, 0);
        }
    }

    // call this to start DEMOMODE
    public void StartAutoMobile() { // called from other script to run in DEMOMODE
        StartCoroutine(AutoMobile());
    }


    #region DEMOMODE this is DEMOAVOIDANCE mode
    /**
    * AutoMobile is the avoidance routine brains.
    * Watch out for zombies!
    */
    IEnumerator AutoMobile() {
        Stop();
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
                            if (turnRight) Right();
                            else Left();
                            turnStart = now + (long)(TURN_TIME * 1000); ;
                            yield return new WaitForSeconds(TURN_TIME);
                        }
                        else {     // back it up!
                            Reverse();
                        }
                    }
                    else  {  // normal turn
                        if (now - turnStart >= SEARCH_TIME_MS) { // turn times out?
                            reversing = true;
                            moveStart = DateTime.Now.Ticks / TimeSpan.TicksPerMillisecond;
                        }
                        else // spin right round!
                        {
                            if (turnRight) Right();
                            else Left();
                        }
                    }
                }
                else // all is clear -- done searching
                {
                    Stop();
                    searching = false;
                    moveStart = now;
                }
            }
            else // going (not searching)
            {
                // too close!
                if (ping > 0 && ping < PING_TOO_CLOSE) 
                {
                    Stop();
                    searching = true;
                    reversing = false;
                    turnStart = DateTime.Now.Ticks / TimeSpan.TicksPerMillisecond;
                    turnRight = (random.Next(0, 2) == 0);
                }
                // tired of going this way?
                else if (now - moveStart >= FORWARD_TIME_MS) 
                {
                    turnRight = (random.Next(0, 2) == 0);
                    if (turnRight) Right();
                    else Left();
                    moveStart = now + (long)(TURN_TIME * 1000);
                    yield return new WaitForSeconds(TURN_TIME);
                }
                else // proceed
                { 
                    Forward();
                }
            }

            yield return null;
        }
    }

    #endregion DEMOMODE

    #region DRIVEMODE Wheel Control Available to all Scripts
    public void Stop()
    {
        driveL = 0;
        driveR = 0;
    }

    /**
     * Move forward
     */
    public void Forward()
    {
        driveL = 250;
        driveR = 250;
    }

    /**
     * Move backward
     */
    public void Reverse()
    {
        driveL = -250;
        driveR = -250;
    }

    /**
     * Turn left
     */
    public void Left()
    {
        driveL = -250;
        driveR = 250;
    }

    /**
     * Turn right
     */
    public void Right()
    {
        driveL = 250;
        driveR = -250;
    }
    #endregion DRIVEMODE
}