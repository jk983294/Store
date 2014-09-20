package com.victor.midas.worker.common;

import com.victor.midas.services.MktDataLoadService;
import com.victor.midas.worker.task.CreateCollectionTask;
import com.victor.midas.worker.task.DeleteStockCollTask;
import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.core.task.TaskExecutor;

import com.victor.midas.dao.TaskDao;
import com.victor.midas.dao.StockDao;

public class TaskMgr {

    private static final Logger logger = Logger.getLogger(TaskMgr.class);

	@Autowired
    private StockDao stockDao;
	@Autowired
    private TaskDao taskDao;
	@Autowired
    private MktDataLoadService mktloader;

    private TaskExecutor taskExecutor;

    public TaskMgr(TaskExecutor taskExecutor) {
        this.taskExecutor = taskExecutor;
    }
    
    /**
     * take the instruction for different command
     * @param instruction
     */
    public void cmd(String instruction){
    	switch(instruction){
    	case "deleteStocks" :  addTask( new DeleteStockCollTask( taskDao , stockDao ) );  break;
    	case "updateStocks" : addTask( new CreateCollectionTask( taskDao , mktloader ) ); break;
    	default : logger.error("no such cmd in task manager.");
    	}
    }

    public void addTask(TaskBase task) {
    	taskExecutor.execute(task);
    }

}
