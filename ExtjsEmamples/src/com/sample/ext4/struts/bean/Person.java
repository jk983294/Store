package com.sample.ext4.struts.bean;

public class Person {
	private String name;
	private int age;
	private String sex;
	private String birthday;
	public String getSex() {
		return sex;
	}
	public void setSex(String sex) {
		this.sex = sex;
	}
	public String getBirthday() {
		return birthday;
	}
	public void setBirthday(String birthday) {
		this.birthday = birthday;
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
	public Person(String name, int age, String sex, String birthday) {
		super();
		this.name = name;
		this.age = age;
		this.sex = sex;
		this.birthday = birthday;
	}
}
