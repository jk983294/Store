package com.victor.midas.services.worker.task;

import com.victor.midas.services.worker.common.TaskBase;
import org.apache.log4j.Logger;

import com.victor.midas.dao.*;

public class CreateCollectionTask extends TaskBase {
	private static final Logger logger = Logger.getLogger(CreateCollectionTask.class);
	private static final String description = "Create Collection Task";

    private StockInfoDao stockInfoDao;
    private IndexDao indexDao;
    private MiscDao miscDao;
    private TaskDao taskDao;


	public CreateCollectionTask(TaskDao taskDao, StockInfoDao stockInfoDao, IndexDao indexDao, MiscDao miscDao) {
		super(description, taskDao);
        this.stockInfoDao = stockInfoDao;
        this.indexDao = indexDao;
        this.miscDao = miscDao;
        this.taskDao = taskDao;
	}

	@Override
	public void doTask() throws Exception {
		taskDao.createCollection();
        stockInfoDao.createCollection();
        indexDao.createCollection();
        miscDao.createCollection();
		logger.info(description + " complete...");
	}

}
