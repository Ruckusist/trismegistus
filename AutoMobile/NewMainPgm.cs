using System;
using System.Collections;
using UnityEngine;
using UnityEngine.UI;

public class NewMainPgm : MonoBehaviour {

    private IEnumerator coroutine;

    // UI
    public Text status;                        // General As-Is Status
    public Text ErrorControl;                   // All Errors in One Place
    public Text RawDataStream;                  // DeBug Output stream of Data right off the tap

    
    public string message = "";
    // Use this for initialization
    void Start()
    {
    }

    // Update is called once per frame
    void Update() {
        if (Input.GetKeyDown("space"))
        {
            coroutine = TESTGOCoroutine1();
            StartCoroutine(coroutine);
        }
    }
    

    public IEnumerator TESTGOCoroutine1() {
        while (true) {
            try
            {
               //NetworkSend.message = "255,255";           // this is not the right call
            }
            catch (Exception err)
            {
                print(err.ToString());
                ErrorControl.text = err.ToString();
            }
            status.text = "UpPressed";
            yield return null;
        }
    }

    public IEnumerator TESTGOCoroutine2() {
        while (true) {
            try {
                //NetworkSend.message = "-255,-255";           // this is not the right call
            }
            catch (Exception err) {
                print(err.ToString());
                ErrorControl.text = err.ToString();
            }
            status.text = "DownPressed";
            yield return null;
        }
    }
}
    
