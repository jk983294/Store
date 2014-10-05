package com.victor.midas.services;


import com.victor.midas.dao.IndexDao;
import com.victor.midas.dao.MiscDao;
import com.victor.midas.services.worker.common.TaskBase;
import com.victor.midas.services.worker.task.CreateCollectionTask;
import com.victor.midas.services.worker.task.DeleteStockCollTask;
import com.victor.midas.services.worker.task.MktDataTask;
import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.core.task.TaskExecutor;

import com.victor.midas.dao.TaskDao;
import com.victor.midas.dao.StockInfoDao;
import org.springframework.stereotype.Component;

import javax.annotation.Resource;
import java.util.Map;

@Component
public class TaskMgr {

    private static final Logger logger = Logger.getLogger(TaskMgr.class);

    @Value("${MktDataLoader.TestStockDirPath}")
    String stockdirpath;
    @Value("${MktDataLoader.StockIndexDirPath}")
    String indexdirpath;

	@Autowired
    private StockInfoDao stockInfoDao;
	@Autowired
    private TaskDao taskDao;
    @Autowired
    private IndexDao indexDao;
    @Autowired
    private MiscDao miscDao;

    @Autowired
    private StocksService stocksService;

    @Resource(name="filepath2prefix")
    Map<String,String> filepath2prefix;

    private TaskExecutor taskExecutor;

    @Autowired
    public TaskMgr(TaskExecutor taskExecutor) {
        this.taskExecutor = taskExecutor;
    }
    
    /**
     * take the instruction for different command
     * @param instruction
     */
    public void cmd(String instruction){
    	switch(instruction){
            case "deleteAllData" :  addTask( new DeleteStockCollTask( taskDao , stockInfoDao, indexDao, miscDao ) );  break;
            case "createDB" : addTask( new CreateCollectionTask( taskDao , stockInfoDao, indexDao, miscDao ) ); break;
            case "loadMarketData" : addTask(new MktDataTask(filepath2prefix, taskDao, stocksService)); break;
            default : logger.error("no such cmd in task manager.");
        }
    }

    public void addTask(TaskBase task) {
    	taskExecutor.execute(task);
    }

}
