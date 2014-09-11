package com.victor.midas.worker;

import org.apache.log4j.Logger;

import com.victor.midas.dao.StockDao;
import com.victor.midas.dao.TaskDao;

public class DeleteStockCollTask extends TaskBase{
	private StockDao stockDao;
	private static final Logger logger = Logger.getLogger(DeleteStockCollTask.class);
	private static final String description = "Delete Collection Task";
	

	public DeleteStockCollTask( TaskDao taskdao , StockDao stockDao ) {
		super(description, taskdao);
		this.stockDao = stockDao;
	}

	@Override
	public void doTask() {
		stockDao.dropStockCollection();
		logger.info(description + " complete...");
	}
}
