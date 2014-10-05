package com.victor.midas.services.worker.common;

import java.sql.Timestamp;

import org.apache.log4j.Logger;

import com.victor.midas.dao.TaskDao;
import com.victor.midas.model.db.TaskDb;

public abstract class TaskBase implements Runnable{
	
	private TaskDb taskDb;
	/**
	 * make taskDao public, so derived class can use it
	 */
	public TaskDao taskDao;
	
	private static final Logger logger = Logger.getLogger(TaskBase.class);
	
	public TaskBase(String desc,TaskDao taskDao) {
		taskDb = new TaskDb(desc);
		this.taskDao = taskDao;
	}

	/**
	 * framework to run a task, handle status change, and responsible for DB serialization
	 */
	public void run() {
		taskDb.setStatus(TaskStatus.Execute);
		taskDao.saveTask(taskDb);		// save task to DB
		try {
			doTask();							
		} catch (Exception e) {
			logger.error(e);
            taskDb.setFailInfo(e.toString());
			taskDb.setStatus(TaskStatus.Error);
		}finally{
			taskDb.setFinish(new Timestamp(System.currentTimeMillis()));
			if(taskDb.getStatus() != TaskStatus.Error) taskDb.setStatus(TaskStatus.Finished);
			taskDao.saveTask(taskDb);
		}		
	}
	
	/**
	 * concrete sub task should implement its business logic 
	 * should not swallow exception, it should be handled by run framework
	 * @throws Exception
	 */
	public abstract void doTask() throws Exception;
}
