//#region Copywrite
//// AutoMobile library collection
//// Oct-2015 by Eric Petersen <Ruckusist@outlook.com>
//// Updates should always be available at https://github.com/Ruckusist/trismegistus
////
//// Changelog:
////     ... - ongoing debug release

//// NOTE: THIS IS ONLY A PARIAL RELEASE. THIS DEVICE CLASS IS CURRENTLY UNDERGOING ACTIVE
//// DEVELOPMENT AND IS STILL MISSING SOME IMPORTANT FEATURES. PLEASE KEEP THIS IN MIND IF
//// YOU DECIDE TO USE THIS PARTICULAR CODE FOR ANYTHING.

///* ============================================
//AutoMobile  library code is placed under the MIT license
//Copyright (c) 2015 Eric Petersen

//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:

//The above copyright notice and this permission notice shall be included in
//all copies or substantial portions of the Software.

//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//THE SOFTWARE.
//===============================================
//*/
//#endregion Copywrite

//#region using
//using UnityEngine;
//using System.Collections;
//#endregion using

///* ---------- This will become a utility to be called from the object to moves script


//    THis is only a test
//    --------------------------------------------- */

//[RequireComponent(typeof(Network_Protocols))]
//public class RealRigidMotion : MonoBehaviour {

//    //Rigidbody rb;
//    Network_Protocols _network;
//    //public float speed;
//    enum accelAxis {
//        Normal = 0,
//        Switched,
//        MAX
//    }

//    Vector3[] horizontalAxes = new Vector3[(int)accelAxis.MAX];
//    Vector3[] verticalAxes = new Vector3[(int)accelAxis.MAX];

//    accelAxis currentAxis = accelAxis.Normal;

//    //Axes
//    float accelX;
//    float accelY;
//    float accelZ;
//    float gyroX;
//    float gyroY;
//    float gyroZ;


//    #region Setup
//    void Start() {
//        _network = GetComponent<Network_Protocols>();
//        getAxes();

//        horizontalAxes[0] = new Vector3(accelX, 0f, 0f);  // setup input on the 1 positive
//        horizontalAxes[1] = new Vector3(0f, 0f,accelZ);  // setup input on the 1 negitive

//        verticalAxes[0] = new Vector3(0f, accelY, 0f);
//        verticalAxes[1] = new Vector3(accelX, 0f, 0f);
//    }

//    void getAxes() {
//        string _data = _network.IncomingMessage;
//        if (_data == null || _data.Length < 1) { return; }  // this never happens...
//        string[] data = _data.Split(',');                 // this can be changed to better arange the data ... but ... this is fine...
//        if (data.Length >= 10) {
//            //accelX = data[3].TryParse() ;
//        }
//    }

//    #endregion Setup

//    #region Operational

//    public void motion(Rigidbody rb, float speed) {
//        // get input
//        float moveHorizontal = Input.GetAxis("accelX");
//        float moveVeritical = Input.GetAxis("accelY");

//        // set input on appropriate axes
//        Vector3 horizontalMovement = horizontalAxes[(int)currentAxis] * moveHorizontal;
//        Vector3 verticalMovement = verticalAxes[(int)currentAxis] * moveVeritical;

//        // get total movement
//        Vector3 movement = horizontalMovement + verticalMovement;

//        rb.AddForce(movement * speed);
//    }

//    #endregion Operational
//}
