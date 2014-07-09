package com.sample.format.xstream;

public class Person {
	private String name;
	private int age;
	private PhoneNumber homePhone;
	private PhoneNumber officePhone;
	public Person(String name, int age, PhoneNumber homePhone,
			PhoneNumber officePhone) {
		super();
		this.name = name;
		this.age = age;
		this.homePhone = homePhone;
		this.officePhone = officePhone;
	}
}
