using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class test : MonoBehaviour {

	public GameObject client;
	public int[] borrowm;
	public static bool returnm;
	public int bankmoney=1000;
	private int outm=0;
	public int lendout;
	public static bool movv;
	public static bool newclientok = false;
	public static int clientnow;
	public static GameObject childclient;
	private string[] clnames;
	public static GameObject okclient;
		
	// Use this for initialization
	void Start () {
		/*
		childclient = Instantiate (client,transform.position,transform.rotation);
		childclient.name = "ball" + i;
		borrowm[i] = Random.Range (100, 1000);
		*/
		returnm  = false;
		clnames = new string[50];

	}
	
	// Update is called once per frame
	void Update () {

		clientnow = mqttTest.clientnow;
		bankmoney = mqttTest.bankmoney;
		returnm = mqttTest.returnmoney;
		newclientok = mqttTest.newclient;

		if (newclientok == true) {
			
			Debug.Log ("new client");
			Debug.Log(mqttTest.clientnow);
			childclient = Instantiate (client,transform.position,transform.rotation);
			childclient.transform.localPosition = new Vector3(0,1,(2*clientnow)-20.5f);
			childclient.name = "ball" + mqttTest.clientnow;
			clnames[mqttTest.clientnow] = childclient.name;
			newclientok = false;
			mqttTest.newclient = false;
		}


		if (returnm == true) {
			string arr = clnames [mqttTest.clientnow];
			okclient=GameObject.Find (arr);
//			Debug.Log ("client name is :" + okclient.name);
			movv = true;
			ex();
			returnm = false;
			mqttTest.returnmoney = false;
		}

	}

	IEnumerable ex()
	{
		yield return new WaitForSeconds (3);
	}

/*	void addclient()
	{
		if (mqttTest.newclient == true) {
			Debug.Log ("new client");
			i++;
			childclient = Instantiate (client,transform.position,transform.rotation);
			childclient.name = "ball" + i;
			newclientok = true;
			mqttTest.newclient = false;
		}
	}*/
}
