using UnityEngine;
using System.Collections;
using UnityEditor;

[CustomEditor(typeof(Robot))]
public class RobotInspector : Editor {

    public override void OnInspectorGUI() {
        
        Robot ROBOT = (Robot)target;  // this is creating a reference to the script making its calls THIS. whatever

        // Changeable Fields
        #region Changeable_Fields
        ROBOT.PING_TOO_CLOSE = EditorGUILayout.IntField("Ping Min: ", ROBOT.PING_TOO_CLOSE);
        ROBOT.PING_PROCEED = EditorGUILayout.IntField("Ping Acceptable: ", ROBOT.PING_PROCEED);
        ROBOT.REAR_PING_TOO_CLOSE = EditorGUILayout.IntField("Rear Ping Min: ", ROBOT.REAR_PING_TOO_CLOSE);
        ROBOT.FORWARD_TIME_MS = EditorGUILayout.IntField("Continue Timer: ", ROBOT.FORWARD_TIME_MS);
        ROBOT.SEARCH_TIME_MS = EditorGUILayout.IntField("Search Timer: ", ROBOT.SEARCH_TIME_MS);
        ROBOT.REVERSE_TIME_MS = EditorGUILayout.IntField("Reverse Timer: ", ROBOT.REVERSE_TIME_MS);
        #endregion Changeable_Fields

        // Readable Fields
        #region Data_Display
        EditorGUILayout.LabelField("Accel X: ", ROBOT.accelX.ToString());
        EditorGUILayout.LabelField("Accel Y: ", ROBOT.accelY.ToString());
        EditorGUILayout.LabelField("Accel Z: ", ROBOT.accelZ.ToString());
        
        EditorGUILayout.LabelField("Ping: ", ROBOT.ping.ToString());
        EditorGUILayout.LabelField("Rear Ping: ", ROBOT.pingRear.ToString());

        EditorGUILayout.LabelField("Incoming: ", ROBOT.INCOMING);

        EditorGUILayout.LabelField("Left Motor Control: ", ROBOT.driveL.ToString());
        EditorGUILayout.LabelField("Right Motor Conrol: ", ROBOT.driveR.ToString());
        #endregion Data_Display

        //if (DrawDefaultInspector()) {        }   // this turns on the Default Inspector

        if (GUILayout.Button("Can Haz MapZ")) {
            
        }
    }
}
