using UnityEngine;
using System.Collections;
using System.Net;
using uPLibrary.Networking.M2Mqtt;
using uPLibrary.Networking.M2Mqtt.Messages;
using uPLibrary.Networking.M2Mqtt.Utility;
using uPLibrary.Networking.M2Mqtt.Exceptions;
using System.Text.RegularExpressions;


using System;

public class mqttTest : MonoBehaviour {
	
	private MqttClient client;
	public static int indexclients;
	public static int borrowmoney;
	public static bool returnmoney =false;
	public static int bankmoney;
	private string strrecv;
	public static int clientnow;
	public static bool reset = false;
	public static bool newclient=false;
	public static int[] borrows;
	public static bool lend = false;
	public static int amout;
	public static bool borr=false;

	// Use this for initialization
	void Start () {
		// create client instance 
		client = new MqttClient(IPAddress.Parse("140.119.164.157"),1883 , false , null ); 
		
		// register to message received 
		client.MqttMsgPublishReceived += client_MqttMsgPublishReceived; 
		
		string clientId = Guid.NewGuid().ToString(); 
		//client.Connect(clientId); 
		
		// subscribe to the topic "/home/temperature" with QoS 2 
		client.Subscribe(new string[] { "banker" }, new byte[] { MqttMsgBase.QOS_LEVEL_EXACTLY_ONCE });
		//client.Subscribe(new string[] { "hello/world" }, new byte[] { MqttMsgBase.QOS_LEVEL_EXACTLY_ONCE }); 
		client.Connect(clientId); 

		//----------------------------------------------------------------------

		borrows = new int[50];

	}
	void client_MqttMsgPublishReceived(object sender, MqttMsgPublishEventArgs e) 
	{ 
		Debug.Log("Received: " + System.Text.Encoding.UTF8.GetString(e.Message) );
		//print (System.Text.Encoding.UTF8.GetString (e.Message));
		if (e.Topic == "banker") {
			strrecv = System.Text.Encoding.Default.GetString (e.Message);
			msgproce ();
		}
		//Debug.Log("Received: " + System.Text.Encoding.UTF8.GetString(e.Message)  );
	}
	private void msgproce()
	{
		newclient = test.newclientok;
		if (strrecv.Substring (0, 3) == "RES") {
			string[] array4 = strrecv.Split (' ');	
			Int32.TryParse (array4 [1], out bankmoney);
			reset = true;
		}
		if (strrecv.Substring (0, 3) == "NEW") {
			string[] array = strrecv.Split(' ');
			Int32.TryParse (array [1], out borrowmoney);
			Int32.TryParse (array [2], out clientnow);
			newclient = true;
			borr = true;
		}
		if (strrecv.Substring (0, 3) == "LEN") {
			string[] array1 = strrecv.Split(' ');
			Int32.TryParse (array1 [1], out clientnow);
			Int32.TryParse (array1 [2], out amout);
			lend = true;
//			borrows [clientnow] += amout;
			bankmoney -= amout;
		}
		if (strrecv.Substring (0, 3) == "CLI") {
			string[] array3 = strrecv.Split(' ');
			Int32.TryParse (array3 [1], out clientnow);
			returnmoney = true;
			print ("out : " + clientnow);
			bankmoney += borrows[clientnow];
		}
	}

	void OnGUI(){
		if ( GUI.Button (new Rect (20,40,80,20), "Level 1")) {
			Debug.Log("sending...");
			client.Publish("banker", System.Text.Encoding.UTF8.GetBytes("Sending from Unity3D!!!"), MqttMsgBase.QOS_LEVEL_EXACTLY_ONCE, true);
			Debug.Log("sent");
		}
	}
	// Update is called once per frame
	void Update () {



	}
}
