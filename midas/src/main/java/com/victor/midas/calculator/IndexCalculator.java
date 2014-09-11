package com.victor.midas.calculator;

import java.util.ArrayList;

import com.victor.midas.model.Record;
import com.victor.midas.model.Stock;

public class IndexCalculator {
	
	public void calculate(Stock stock) {
		change(stock);
	}
	
	/**
	 * calculate change percentage per day
	 * @param stock
	 */
	public void change(Stock stock){
		ArrayList<Record> records = stock.getRecords();
		int len = records.size();
		for (int i = 1; i < len; i++) {
			records.get(i).setChange((records.get(i).getEnd() - records.get(i-1).getEnd())/records.get(i-1).getEnd());
		}
	}
}
