package com.victor.midas.worker.task;

import com.victor.midas.services.MktDataLoadService;
import com.victor.midas.worker.common.TaskBase;
import org.apache.log4j.Logger;

import com.victor.midas.dao.TaskDao;

public class CreateCollectionTask extends TaskBase {
	private static final Logger logger = Logger.getLogger(CreateCollectionTask.class);
	private static final String description = "Create Collection Task";
	
	private MktDataLoadService mktloader;

	public CreateCollectionTask(TaskDao admindao ,MktDataLoadService mktloader) {
		super(description, admindao);
		this.mktloader = mktloader;
	}

	@Override
	public void doTask() throws Exception {
		taskDao.createCollection();
		mktloader.saveAllFromStockDirPath();
		logger.info(description + " complete...");
	}

}
