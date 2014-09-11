package com.victor.midas.worker;

import java.sql.Timestamp;

import org.apache.log4j.Logger;

import com.victor.midas.dao.TaskDao;
import com.victor.midas.model.Task;

public abstract class TaskBase implements Runnable{
	
	private Task task;
	/**
	 * make taskDao public, so derived class can use it
	 */
	public TaskDao taskDao;
	
	private static final Logger logger = Logger.getLogger(TaskBase.class);
	
	public TaskBase(String desc,TaskDao taskDao) {
		task = new Task(desc);
		this.taskDao = taskDao;
	}

	/**
	 * framework to run a task, handle status change, and responsible for DB serialization
	 */
	public void run() {
		task.setStatus(TaskStatus.Execute);
		taskDao.saveTask(task);		// save task to DB
		try {
			doTask();							
		} catch (Exception e) {
			logger.error(e);
			task.setStatus(TaskStatus.Error);
		}finally{
			task.setFinish(new Timestamp(System.currentTimeMillis()));
			if(task.getStatus() != TaskStatus.Error) task.setStatus(TaskStatus.Finished);
			taskDao.saveTask(task);
		}		
	}
	
	/**
	 * concrete sub task should implement its business logic 
	 * should not swallow exception, it should be handled by run framework
	 * @throws Exception
	 */
	public abstract void doTask() throws Exception;
}
