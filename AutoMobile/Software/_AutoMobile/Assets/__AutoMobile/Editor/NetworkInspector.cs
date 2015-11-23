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

[CustomEditor(typeof(Network_Protocols))]
public class NetworkInspector : Editor {

    public override void OnInspectorGUI() {
        #region Startup
        Network_Protocols network = (Network_Protocols)target;

        //string _data = "192.168.2.177,303,141,-619,-51,4780,22,0,4820,0";
        
        string _data = network.IncomingMessage;
        //EditorGUILayout.TextArea("Raw Data Stream::: " + _data);
        #endregion Startup

        

        #region Telemetry
        if (_data == null || _data.Length < 1) { _data = "192.168.2.177,303,141,-619,-51,4780,22,0,4820,0"; }  // this never happens...
        string[] data = _data.Split(',');                 // this can be changed to better arange the data ... but ... this is fine...
        int _length = data.Length;
        if (data.Length <= _length) {

            //EditorGUILayout.LabelField("           Incoming IP:  ");
            //EditorGUILayout.LabelField("          " + data[0]);
            EditorGUILayout.LabelField("           Telemetry:  ");
            EditorGUILayout.LabelField("Audio: " + data[0]);
            EditorGUILayout.LabelField("Ping: " + data[1]);
            EditorGUILayout.LabelField("Ping Rear: " + data[8]);   // Maybe put this in its proper spot
            EditorGUILayout.LabelField("Accel of x ::: " + data[2]);
            EditorGUILayout.LabelField("Accel of y ::: " + data[3]);
            EditorGUILayout.LabelField("Accel of z ::: " + data[4]);
            EditorGUILayout.LabelField("Gyro of x ::: " + data[5]);
            EditorGUILayout.LabelField("Gyro of y ::: " + data[6]);
            EditorGUILayout.LabelField("Gyro of z ::: " + data[7]);
            //network.IncomingIP = data[0];
        }


        //EditorGUILayout.LabelField("           Incoming IP:  ");
        //EditorGUILayout.LabelField("          " + data[0]);
        //EditorGUILayout.LabelField("           Telemetry:  ");
        //EditorGUILayout.LabelField("Audio: " + data[1]);
        //EditorGUILayout.LabelField("Ping: " + data[2]);
        //EditorGUILayout.LabelField("Ping Rear: " + data[9]);   // Maybe put this in its proper spot
        //EditorGUILayout.LabelField("Accel of x ::: " + data[3]);
        //EditorGUILayout.LabelField("Accel of y ::: " + data[4]);
        //EditorGUILayout.LabelField("Accel of z ::: " + data[5]);
        //EditorGUILayout.LabelField("Gyro of x ::: " + data[6]);
        //EditorGUILayout.LabelField("Gyro of y ::: " + data[7]);
        //EditorGUILayout.LabelField("Gyro of z ::: " + data[8]);
        //network.IncomingIP = data[0];

        //EditorGUILayout.LabelField("           Sent Data:   ");
        //EditorGUILayout.LabelField("  Motor Left: " + network.DriveL);
        //EditorGUILayout.LabelField("  Motor Right: " + network.DriveR);
        #endregion Telemetry

        #region BUTTONS

        if (GUILayout.Button("Start Network Recieve")) {
            network.StartCoroutine(network.NetRecRoutine()); // this works
        }
        if (GUILayout.Button("Start Network Send")) {
            network.StartCoroutine(network.SendRoutine()); // this works
        }
        if (GUILayout.Button("Start Fly")) {
            //util.StartCoroutine(util.FlyAccelGyro()); // this DOESNT works
        }
        if (GUILayout.Button("Stop All Action")) {
            //util.StopAllCoroutines();
            network.StopAllCoroutines();
            _data = "";
        }
        if (GUILayout.Button("Refesh")) {
            //EditorUtility.SetDirty( target );
        }
        #endregion BUTTONS 

        // Refesher
        EditorUtility.SetDirty( target );
        Repaint();
        RequiresConstantRepaint();
    }
}