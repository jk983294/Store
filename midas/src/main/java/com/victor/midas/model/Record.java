package com.victor.midas.model;

import java.util.Date;

public class Record {
	private double start;			// start price
	private double end;				// end price
	private double max;				// max price
	private double min;				// min price
	private int volume;				
	private double total;			// total = volume * end
	private Date date;
	private double change;
	
	public Record(){
		super();
	}
	
	public Record(double start, double end, double max, double min, int volume,
			double total, Date date) {
		super();
		this.start = start;
		this.end = end;
		this.max = max;
		this.min = min;
		this.volume = volume;
		this.total = total;
		this.date = date;
	}

	public double getStart() {
		return start;
	}
	public void setStart(double start) {
		this.start = start;
	}
	public double getEnd() {
		return end;
	}
	public void setEnd(double end) {
		this.end = end;
	}
	public double getMax() {
		return max;
	}
	public void setMax(double max) {
		this.max = max;
	}
	public double getMin() {
		return min;
	}
	public void setMin(double min) {
		this.min = min;
	}
	public int getVolume() {
		return volume;
	}
	public void setVolume(int volume) {
		this.volume = volume;
	}
	public double getTotal() {
		return total;
	}
	public void setTotal(double total) {
		this.total = total;
	}
	public Date getDate() {
		return date;
	}
	public void setDate(Date date) {
		this.date = date;
	}
	public double getChange() {
		return change;
	}

	public void setChange(double change) {
		this.change = change;
	}

	@Override
	public String toString() {
		return "Record [start=" + start + ", end=" + end + ", max=" + max
				+ ", min=" + min + ", volume=" + volume + ", total=" + total
				+ ", date=" + date + ", change=" + change + "]";
	}

	
	
	
	
}
