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
#endregion using

public class RoboAction : MonoBehaviour {

    #region Prefs
    //Button Send Prefs
    int counter1;
    int counter2;
    float TargetMotorSpeedL;
    float TargetMotorSpeedR;
    float RealMotorSpeedL = 0;                              // actual transmited speeds
    float RealMotorSpeedR = 0;                              // same
    
    #endregion Prefs

    #region Button_Send
    public void Go(int L, int R) {
        TargetMotorSpeedL = L;
        TargetMotorSpeedR = R;
        _speedRamping();
    }

    public void _speedRamping() {
        // Left Side Speed Ramping
        if (RealMotorSpeedL != TargetMotorSpeedL) {
            if (TargetMotorSpeedL > RealMotorSpeedL) {
                RealMotorSpeedL++;
                if (RealMotorSpeedL == 1) {
                    RealMotorSpeedL += 100;
                }
                if (RealMotorSpeedL == -100) {
                    RealMotorSpeedL += 100;
                }
            }
            if (TargetMotorSpeedL < RealMotorSpeedL) {
                RealMotorSpeedL--;
                if (RealMotorSpeedL == -1) {
                    RealMotorSpeedL -= 101;
                }
                if (RealMotorSpeedL == 100) {
                    RealMotorSpeedL -= 100;
                }
            }
        }
        // Right Side Motor Ramping
        if (RealMotorSpeedR != TargetMotorSpeedR) {
            if (TargetMotorSpeedR > RealMotorSpeedR) {
                RealMotorSpeedR++;
                if (RealMotorSpeedR == 1) {
                    RealMotorSpeedR += 100;
                }
                if (RealMotorSpeedR == -100) {
                    RealMotorSpeedR += 100;
                }

            }
            if (TargetMotorSpeedR < RealMotorSpeedR) {
                RealMotorSpeedR--;
                if (RealMotorSpeedR == -1) {
                    RealMotorSpeedR -= 101;
                }
                if (RealMotorSpeedR == 100) {
                    RealMotorSpeedR -= 100;
                }
            }
        }

        //final collect and Communicate
        counter1 = (int)RealMotorSpeedL;
        counter2 = (int)RealMotorSpeedR;
        TalkBack(counter1, counter2);
    }
    
    // Communicate
    void TalkBack(int Left, int Right) {
        GetComponent<Network_Protocols>().DriveL = Left;
        GetComponent<Network_Protocols>().DriveR = Right;
    }
    #endregion Button_Send
}
