using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class move : MonoBehaviour {

	public static bool isok;
	public float speed=0.1f;
	private GameObject client;
	private int clnow;

	// Use this for initialization
	void Start () {

	}
	
	// Update is called once per frame
	void Update () {
		client = test.okclient;
		clnow = test.clientnow;
		isok = test.movv;
		if (isok == true) {
			if (client != null) {
				client.transform.Translate (-speed, 0, 0);
			}
		}
	}

	private void OnTriggerEnter(Collider col){
		if (col.CompareTag ("wall") == true) {
			Debug.Log ("move client: " + this.name);
			isok = false;
			test.movv = false;
			Destroy (this.gameObject);
		}
	}
}
