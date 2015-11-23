using UnityEngine;
using System.Collections;

public class Camfollower : MonoBehaviour {

    #region PREFS for the Following RoboCam
    //public static Camfollow instance; // for access to instance data by other scripts
    public GameObject target;

    public float xOffset = 0;
    public float yOffset = 0;
    public float zOffset = 0;
    public int zoom = 40;
    public int normal = 60;
    public float smooth = 5;
    public bool isZoomed = false;
    #endregion PREFS

    void Update() {
        if (Input.GetKeyDown("z")) {
            isZoomed = !isZoomed;
        }

        if (isZoomed == true) {
            GetComponent<Camera>().fieldOfView = Mathf.Lerp(GetComponent<Camera>().fieldOfView, zoom, Time.deltaTime * smooth);
        }
        else {
            GetComponent<Camera>().fieldOfView = Mathf.Lerp(GetComponent<Camera>().fieldOfView, normal, Time.deltaTime * smooth);
        }
    }

    void LateUpdate() {
        transform.position = new Vector3(target.transform.position.x + xOffset,
                                         target.transform.position.y + yOffset,
                                         target.transform.position.z + zOffset);
        //this.transform.rotation = Quaternion.identity;
    }
}
