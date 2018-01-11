using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class control : MonoBehaviour {

	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		
	}

	private void OnTriggerEnter(Collider col){
		if (col.CompareTag ("wall") == true) {
			isok = false;
			Destroy (this.gameObject);
		}
	}
}
