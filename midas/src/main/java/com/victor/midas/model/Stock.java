package com.victor.midas.model;

import java.util.ArrayList;

import org.springframework.data.annotation.*;

public class Stock {
	
	@Id
	private String id;
	private String name;
	private String desp;
	private ArrayList<Record> records;

	
	public Stock() {
		records = new ArrayList<Record>();
	}
	
	public void addRecord(Record record){
		records.add(record);
	}
	
	public String getId() {
		return id;
	}

	public void setId(String id) {
		this.id = id;
	}

	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public String getDesp() {
		return desp;
	}
	public void setDesp(String desp) {
		this.desp = desp;
	}
	public ArrayList<Record> getRecords() {
		return records;
	}
	public void setRecords(ArrayList<Record> records) {
		this.records = records;
	}

	@Override
	public String toString() {
		return "Stock [name=" + name + ", desp=" + desp + ", records="
				+ records + "]";
	}
	
	
}
