package com.sample.format.jsonlib;

import net.sf.json.JSONObject;

public class JsonLibTest {
	public static void main(String[] args){
		PhoneNumber homePhone = new PhoneNumber("宅电","123456");
		PhoneNumber officePhone = new PhoneNumber("办公电话","654321");
		Person person = new Person("tom",20,homePhone,officePhone);
		JSONObject json = JSONObject.fromObject(person);
		String jsonStr = json.toString();
		System.out.println(json);
	}
}
