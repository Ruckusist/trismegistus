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

public class DirButtons : MonoBehaviour {

    #region Builds
    public GameObject wBtn, sBtn, aBtn, dBtn, spBtn;
    Network_Protocols _nework;
    RoboAction _robo;
    Color initColor;
    #endregion Builds
    
    #region Startup
    void Start() {
        _robo = FindObjectOfType<RoboAction>();
    }
    #endregion Startup

    #region Updates // Button Press Commands
    void Update() {
        if (Input.GetButton("F_key")) {
            //     this is a cool custom button setup in the inspector
            //print("f key was pressed");
            //ColorChangeBtn(wBtn, true);
            //Vector3 moveInput = new Vector3(Input.GetAxisRaw("Horizontal"), 0, Input.GetAxisRaw("Vertical"));
            //Vector3 moveVelocity = moveInput.normalized * moveSpeed;
            //_robo._MOVE(moveVelocity);
        }
        if (Input.GetKey("w")) {
            ColorChangeBtn(wBtn, true);
            //print("w key was depressed");
            _robo.Go(255,255);
        }
        if (Input.GetKey("a")) {
            //print("a key was pressed");
            ColorChangeBtn(aBtn, true);
            _robo.Go(-255,255);
        }
        if (Input.GetKey("s")) {
            //print("s key was pressed");
            ColorChangeBtn(sBtn, true);
            _robo.Go(-255,-255);
        }
        if (Input.GetKey("d")) {
            //print("d key was pressed");
            ColorChangeBtn(dBtn, true);
            _robo.Go(255,-255);
        }
        if (Input.GetKey("space")) {
            //print("space key was pressed");
            ColorChangeBtn(spBtn, true);
            _robo.Go(0,0);
        }
        if (!Input.anyKey) {
            _robo.Go(0,0);
        }
    }
    #endregion Updates

    #region Visual
    void ColorChangeBtn(GameObject button, bool _switch) {
        Material ButtonStart = wBtn.GetComponent<Renderer>().material;
        Color StartColor = ButtonStart.color;
        Material Button = button.GetComponent<Renderer>().material;
        //Color initColor = Color.blue;
        Color FlashingColor = new Color(Random.Range(0f, 1f), Random.Range(0f, 1f), Random.Range(0f, 1f), 1);
        Button.color = Color.Lerp(StartColor, FlashingColor, Mathf.PingPong(Random.Range(0f, 1f), 1));
    }
    #endregion Visual
}
