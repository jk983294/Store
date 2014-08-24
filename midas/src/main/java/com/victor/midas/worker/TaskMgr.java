package com.victor.midas.worker;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.core.task.TaskExecutor;

import com.victor.midas.dao.TaskDao;
import com.victor.midas.dao.StockDao;

public class TaskMgr {
	@Autowired
    private StockDao stockDao;
	@Autowired
    private TaskDao taskDao;
	
	@Autowired
    private MktDataLoader mktloader;

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
    	default : 
    	}
    }

    public void addTask(TaskBase task) {
    	taskExecutor.execute(task);
    }

}
