package com.victor.midas.services.worker.task;

import com.victor.midas.dao.StockDao;
import com.victor.midas.dao.MiscDao;
import com.victor.midas.services.worker.common.TaskBase;
import org.apache.log4j.Logger;

import com.victor.midas.dao.StockInfoDao;
import com.victor.midas.dao.TaskDao;

public class DeleteStockCollTask extends TaskBase {
	private static final Logger logger = Logger.getLogger(DeleteStockCollTask.class);
	private static final String description = "Delete Collection Task";

    private StockInfoDao stockInfoDao;
    private StockDao stockDao;
    private MiscDao miscDao;
    private TaskDao taskDao;
	

	public DeleteStockCollTask( TaskDao taskDao , StockInfoDao stockInfoDao, StockDao stockDao, MiscDao miscDao) {
        super(description, taskDao);
        this.stockInfoDao = stockInfoDao;
        this.stockDao = stockDao;
        this.miscDao = miscDao;
        this.taskDao = taskDao;
	}

	@Override
	public void doTask() {
        // clear all documents in task collection
//        taskDao.deleteCollection();
//        taskDao.createCollection();

        stockInfoDao.deleteCollection();
        stockDao.deleteCollection();
        miscDao.deleteCollection();
		logger.info(description + " complete...");
	}
}
