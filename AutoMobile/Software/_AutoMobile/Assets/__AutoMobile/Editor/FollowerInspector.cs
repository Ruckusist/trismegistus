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
using UnityEditor;
#endregion using

[CustomEditor(typeof(Follower))]
public class FollowerInspector : Editor {

    #region Builds

    Follower follower;

    #endregion Builds

    #region Prefs
    #endregion Prefs

    #region Startup
    public override void OnInspectorGUI() {

        //Follower follower = (Follower)target;

        if (GUILayout.Button("Follow")) {
            //follower.StartCoroutine(follower.Follow());
        }
        if (GUILayout.Button("Stop Follow")) {
            //follower.StopCoroutine(follower.Follow());
        }
        #endregion Startup
    }
}
