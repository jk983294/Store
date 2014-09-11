package com.victor.utilities.lib.commons;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class DemoPojo {
	private String a;
	private Integer b;
	private Map<String, String> values;
	private List<String> list;
	
	public DemoPojo(){}
	
	public DemoPojo(String a, Integer b) {
		this.a = a;
		this.b = b;
	}
	
	public void print(String msg) {
		System.out.println(msg);
	}
	

	public String getA() {
		return a;
	}

	public void setA(String a) {
		this.a = a;
	}

	public Integer getB() {
		return b;
	}

	public void setB(Integer b) {
		this.b = b;
	}

	public Map<String, String> getValues() {
		return values;
	}

	public void setValues(Map<String, String> values) {
		this.values = values;
	}

	public List<String> getList() {
		return list;
	}

	public void setList(List<String> list) {
		this.list = list;
	}

	@Override
	public String toString() {
		return "DemoPojo [a=" + a + ", b=" + b + ", values=" + values
				+ ", list=" + list + "]";
	}
	
	
}
