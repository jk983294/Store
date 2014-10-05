package com.victor.midas.services.worker.task;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import com.victor.midas.calculator.IndexCalculator;
import com.victor.midas.model.vo.StockVo;
import com.victor.midas.services.StocksService;
import com.victor.midas.services.worker.common.TaskBase;
import com.victor.midas.util.MidasConstants;
import org.apache.commons.io.FileUtils;
import org.apache.log4j.Logger;

import com.victor.midas.dao.TaskDao;

public class MktDataTask extends TaskBase {
	
	private static final Logger logger = Logger.getLogger(MktDataTask.class);
	private static final String description = "Market Data Load Task";

	private TaskDao taskdao;
	private StocksService stocksService;
    private Map<String,String> filepath2prefix;
    private List<StockVo> stocks;
	

	public MktDataTask(Map<String,String> filepath2prefix, TaskDao taskdao, StocksService stocksService) {
		super( description  , taskdao);
		this.taskdao = taskdao;
		this.stocksService = stocksService;
		this.filepath2prefix = filepath2prefix;
        stocks = new ArrayList<>();
	}

	@Override
	public void doTask() throws IOException {
        for (Map.Entry<String, String> entry : filepath2prefix.entrySet()) {
            String filePath = entry.getKey().toString();
            String prefix = entry.getValue().toString();
            fromDirectory(filePath, prefix);
            System.out.println("key=" + filePath + " value=" + prefix);
        }
        calcIndex();
        stocksService.saveStocks(stocks);
		logger.info( description + " complete...");
	}

    public void fromDirectory(String dir,String prefix) throws IOException{
        File root = new File(dir);
        File[] files = root.listFiles();
        for(File file:files){
            if(file.isDirectory()){
                //Recursive call
                fromDirectory(file.getAbsolutePath(),prefix);
            }else{
                String fileName = file.getAbsolutePath();
                if(fileName.endsWith(".TXT")||fileName.endsWith(".txt")){
                    StockVo stock = fromFile(file.getAbsolutePath(), prefix);
					logger.info(stock.getStockName());
                    stocks.add(stock);
                    //stockdao.saveStock(stock);
                }
            }
        }
    }

    private void calcIndex(){
        IndexCalculator indexCalculator = new IndexCalculator(stocks);
        indexCalculator.calculate();
    }
	
	/**
	 * read market data from file
	 * @param path
	 * @return
	 * @throws IOException 
	 */
	public StockVo fromFile(String path, String prefix) throws IOException{
        File file = new File(path);
        List<String> contents = FileUtils.readLines(file, "GBK");
        int cnt = contents.size() - 3;
        double[] start = new double[cnt];
        double[] end = new double[cnt];
        double[] max = new double[cnt];
        double[] min = new double[cnt];
        double[] total = new double[cnt];
        int[] volume = new int[cnt];
        int[] date = new int[cnt];

        for (int i = 0; i < cnt; i++) {
            String arr[] = contents.get( i + 2 ).split("\t");
            date[i] = string2date(arr[0]);
            start[i] = Double.valueOf(arr[1]);
            max[i] = Double.valueOf(arr[2]);
            min[i] = Double.valueOf(arr[3]);
            end[i] = Double.valueOf(arr[4]);
            volume[i] = Integer.valueOf(arr[5]);
            total[i] = Double.valueOf(arr[6]);
        }

        String arr[] = contents.get(0).split(" ");
        String stockName = prefix + arr[0];
        StringBuilder sb = new StringBuilder();
        for (int i = 1; i < arr.length-1; i++) {
            sb.append(arr[i]);
        }

        StockVo stock = new StockVo(stockName, sb.toString());
        stock.addIndex(MidasConstants.INDEX_NAME_DATE, date);
        stock.addIndex(MidasConstants.INDEX_NAME_START, start);
        stock.addIndex(MidasConstants.INDEX_NAME_MAX, max);
        stock.addIndex(MidasConstants.INDEX_NAME_MIN, min);
        stock.addIndex(MidasConstants.INDEX_NAME_END, end);
        stock.addIndex(MidasConstants.INDEX_NAME_VOLUME, volume);
        stock.addIndex(MidasConstants.INDEX_NAME_TOTAL, total);
		return stock;
	}
	
	private int string2date(String str){
        int date = 0;
        date += Integer.valueOf(str.substring(0,4)) * 10000;
        date += Integer.valueOf(str.substring(5,7)) * 100;
        date += Integer.valueOf(str.substring(8,10));
        return date;
    }

    /**
     * pre-fetch all paths
     * maybe used for load balance
     * @return
     * @throws Exception
     */
    public List<String> getAllFilePaths() throws Exception{
        List<String> filepaths = new ArrayList<>();
        for (Map.Entry<String, String> entry : filepath2prefix.entrySet()) {
            String filePath = entry.getKey().toString();
            filepaths.addAll(getAllFilePaths(filePath));
        }
        return filepaths;
    }

    public List<String> getAllFilePaths(String dir) throws Exception{
        File root = new File(dir);
        File[] files = root.listFiles();
        List<String> filepaths = new ArrayList<>();
        for(File file:files){
            if(file.isDirectory()){
                filepaths.addAll(getAllFilePaths(file.getAbsolutePath()));
            }else{
                filepaths.add(file.getAbsolutePath());
            }
        }
        return filepaths;
    }
}
