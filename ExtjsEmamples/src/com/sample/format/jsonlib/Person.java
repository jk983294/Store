package com.sample.format.jsonlib;

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
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public int getAge() {
		return age;
	}
	public void setAge(int age) {
		this.age = age;
	}
	public PhoneNumber getHomePhone() {
		return homePhone;
	}
	public void setHomePhone(PhoneNumber homePhone) {
		this.homePhone = homePhone;
	}
	public PhoneNumber getOfficePhone() {
		return officePhone;
	}
	public void setOfficePhone(PhoneNumber officePhone) {
		this.officePhone = officePhone;
	}
}
