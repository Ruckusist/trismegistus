using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using System;

public class Robot : MonoBehaviour
{

    /** the instance of the robot (car) that Unity and comms will interact with */
    public static Robot instance; // for access to instance data by other scripts


    //-- CONFIGURATION (override in Unity UI) --//

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



    //-- INSTANCE DATA --//

    // wheels
    public int driveL = 0;
    public int driveR = 0;

    // telemetry
    public int ping;
    public int pingRear;
    public int accelX = 0;
    public int accelY;
    public int accelZ;

    // unity visual
    public int speed = 2;

    // brain state
    bool searching = false;
    bool reversing = false;
    bool turnRight = false;
    long moveStart = 0;
    long turnStart = 0;

    System.Random random;

    /**
     * We should be started when Unity begins
     */
    public void Start()
    {
        instance = this;
        random = new System.Random();
    }


    /**
     * Update is called once per frame
     */
    public void Update()
    {
        // backwards
        if (accelX < ACCEL_X_ZERO_MIN)
        {
            transform.Translate(0, 0, -speed * Time.deltaTime);
        }
        // forwards
        else if (accelX > ACCEL_X_ZERO_MAX)
        {
            transform.Translate(0, 0, speed * Time.deltaTime);
        }

        // left
        if (accelY < ACCEL_Y_ZERO_MIN)
        {
            transform.Translate(speed * Time.deltaTime, 0, 0);
        }
        // right
        else if (accelY > ACCEL_Y_ZERO_MAX)
        {
            transform.Translate(-speed * Time.deltaTime, 0, 0);
        }
    }


    /**
     * start the auto-mobile coroutine (by button, usually)
     */
    public void StartAutoMobile()
    {
        StartCoroutine(AutoMobile());
    }

    /**
     * AutoMobile is the avoidance routine brains.
     * Watch out for zombies!
     */
    IEnumerator AutoMobile()
    {
        Stop();
        moveStart = DateTime.Now.Ticks / TimeSpan.TicksPerMillisecond;

        while (true)
        {
            long now = DateTime.Now.Ticks / TimeSpan.TicksPerMillisecond;

            if (searching)
            {
                // too close!
                if (ping >= 0 && ping < PING_TOO_CLOSE)
                {

                    if (reversing)
                    {
                        // ready to stop reversing?
                        if ((pingRear > 0 && pingRear < REAR_PING_TOO_CLOSE) || now - moveStart >= REVERSE_TIME_MS)
                        {
                            reversing = false;
                            turnRight = (random.Next(0, 2) == 0);
                            if (turnRight) Right();
                            else Left();
                            turnStart = now + (long)(TURN_TIME * 1000); ;
                            yield return new WaitForSeconds(TURN_TIME);
                        }
                        else // back it up!
                        {
                            Reverse();
                        }
                    }
                    else // normal turn
                    {
                        if (now - turnStart >= SEARCH_TIME_MS) // turn time out?
                        {
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

    /**
     * Cease all motion
     */
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
}
