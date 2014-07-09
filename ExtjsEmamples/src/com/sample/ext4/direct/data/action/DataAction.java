package com.sample.ext4.direct.data.action;

import java.util.ArrayList;
import java.util.List;

import com.sample.ext4.direct.data.model.Person;
import com.softwarementors.extjs.djn.config.annotations.DirectMethod;

public class DataAction {
	/**
	 * 读取指定id的人员信息
	 * @param id
	 * @return
	 */
	@DirectMethod
	public Person readData(int id){
		Person p = new Person();
		p.setId(id);
		p.setAge(20);
		p.setName("张三");
		return p;
	}
	/**
	 * 读取指定id的人员信息
	 * @param id
	 * @return
	 */
	@DirectMethod
	public List<Person> readAllData(){
		List<Person> persons = new ArrayList<Person>();
		Person p1 = new Person();
		p1.setId(1);
		p1.setAge(20);
		p1.setName("张三");
		persons.add(p1);
		
		Person p2 = new Person();
		p2.setId(2);
		p2.setAge(30);
		p2.setName("李四");
		persons.add(p2);
		
		return persons;
	}
}
