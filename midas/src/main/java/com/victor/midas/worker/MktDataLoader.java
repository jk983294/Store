package com.victor.midas.worker;

import java.io.*;
import java.sql.Date;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;

import com.victor.midas.dao.StockDao;
import com.victor.midas.model.*;

/**
 * load data from file system
 */
public class MktDataLoader {
	@Value("${MktDataLoader.TestStockDirPath}")
	String stockdirpath;
	
	@Autowired
	StockDao stockdao;
	
	public void saveAllFromStockDirPath() throws Exception{
		System.out.println("load raw stocks to mongodb from dir : " + stockdirpath);
		System.out.println("load raw stocks to mongodb from dir : " + stockdao);
		stockdao.createCollection();
		fromDirectory(stockdirpath);
	}
	
	
	public void fromDirectory(String dir) throws Exception{
		File root = new File(dir);
		File[] files = root.listFiles();
		for(File file:files){     
			if(file.isDirectory()){
				//µÝ¹éµ÷ÓÃ
				fromDirectory(file.getAbsolutePath());
			}else{
				String fileName = file.getAbsolutePath();
				if(fileName.endsWith(".TXT")||fileName.endsWith(".txt")){
					Stock stock = fromFile(file.getAbsolutePath());
					System.out.println(stock.getName());
					stockdao.saveStock(stock);
				}				
			}   
		}
	}
	
	/**
	 * read mkdata from file
	 * @param path
	 * @return
	 */
	public Stock fromFile(String path){
		Stock stock = new Stock();
		BufferedReader br = null;
		try {
			br = new BufferedReader(new InputStreamReader(new FileInputStream(new File(path))));
			String line = null;
			if((line = br.readLine()) != null){
				String arr[] = line.split(" ");
				stock.setName(arr[0]);
				stock.setDesp(arr[1]);
				line = br.readLine();			//remove desp line
				while((line = br.readLine()) != null){
					if(line.length() > 20){
						Record record = recordFromSingleLine(line);
						stock.addRecord(record);
					}	
				}
			}
			br.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
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
