package com.victor.midas.worker;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.sql.Date;
import java.util.ArrayList;

import org.apache.log4j.Logger;

import com.victor.midas.dao.TaskDao;
import com.victor.midas.dao.StockDao;
import com.victor.midas.model.Record;
import com.victor.midas.model.Stock;

public class MktDataTask extends TaskBase{
	
	private final Logger logger = Logger.getLogger(MktDataTask.class);
	private static final String description = "Market Data Load Task";
	
	private ArrayList<String> filepath;
	private String prefix;
	private TaskDao admindao;
	private StockDao stockdao;
	

	public MktDataTask(ArrayList<String> filepath, String prefix, 
			TaskDao admindao, StockDao stockdao) {
		super(prefix + " " + description  , admindao);
		this.admindao = admindao;
		this.stockdao = stockdao;
		this.filepath = filepath;
		this.prefix = prefix;
	}

	@Override
	public void doTask() throws IOException {
		for (String path : filepath) {
			Stock stock = fromFile(path);
			stock.setName(prefix + stock.getName());
			logger.info(stock.getName());
			stockdao.saveStock(stock);
		}
		logger.info(prefix + " " + description + " complete...");
	}
	
	/**
	 * read market data from file
	 * @param path
	 * @return
	 * @throws IOException 
	 */
	public Stock fromFile(String path) throws IOException{
		Stock stock = new Stock();
		BufferedReader br = null;
		br = new BufferedReader(new InputStreamReader(new FileInputStream(new File(path))));
		String line = null;
		if((line = br.readLine()) != null){
			String arr[] = line.split(" ");
			stock.setName(arr[0]);
			String desp = "";
			for (int i = 1; i < arr.length-1; i++) {
				desp += arr[i];
			}
			stock.setDesp(desp);
			line = br.readLine();			//remove description line
			while((line = br.readLine()) != null){
				if(line.length() > 20){
					Record record = recordFromSingleLine(line);
					stock.addRecord(record);
				}	
			}
		}
		br.close();
		return stock;
	}
	
	/**
	 * tokenize a line of day record
	 * @param line
	 * @return
	 */
	public Record recordFromSingleLine(String line){
		String arr[] = line.split("\t");
		Record record = new Record();
		record.setDate(Date.valueOf(arr[0]));
		record.setStart(Double.valueOf(arr[1]));
		record.setMax(Double.valueOf(arr[2]));
		record.setMin(Double.valueOf(arr[3]));
		record.setEnd(Double.valueOf(arr[4]));
		record.setVolume(Integer.valueOf(arr[5]));
		record.setTotal(Double.valueOf(arr[6]));
		return record;
	}
}
