package com.sample.format.jsonlib;

import java.util.ArrayList;
import java.util.List;

import com.sample.format.util.ExtHelper;

public class JsonListTest {
	public static void main(String[] args){
		PhoneNumber homePhone = new PhoneNumber("宅电","123456");
		PhoneNumber officePhone = new PhoneNumber("办公电话","654321");
		List phoneList = new ArrayList();
		phoneList.add(homePhone);
		phoneList.add(officePhone);;
		String json = ExtHelper.getJsonFromList(phoneList.size(), phoneList);
		System.out.println(json);
	}
}