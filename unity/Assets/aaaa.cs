using UnityEngine;
using System.Collections;
using System.Net;
using System;
using System.Text.RegularExpressions;

public class aaaa : MonoBehaviour {

	public string a;
	private int x;

	// Use this for initialization
	void Start () {
		a="xxxddddaass 123 333 222";
	}
	
	// Update is called once per frame
	void Update () {
		if (a.Substring (0, 3) == "xxx") {
			string[] array = Regex.Split (a, " ", RegexOptions.IgnoreCase);
			Int32.TryParse (array [1], out x);
			print (x);
		}
	}
}
