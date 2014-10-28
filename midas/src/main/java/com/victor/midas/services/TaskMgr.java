package com.victor.midas.services;


import com.victor.midas.dao.StockDao;
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
    private StockDao stockDao;
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

    public static final String CMD_DELETE_DATE = "deleteAllData";
    public static final String CMD_LOAD_DATA = "loadMarketData";
    public static final String CMD_CREATE_DB = "createDB";
    
    /**
     * take the instruction for different command
     * @param instruction
     */
    public void cmd(String instruction){
    	switch(instruction){
            case CMD_DELETE_DATE :  addTask( new DeleteStockCollTask( taskDao , stockInfoDao, stockDao, miscDao ) );  break;
            case CMD_CREATE_DB : addTask( new CreateCollectionTask( taskDao , stockInfoDao, stockDao, miscDao ) ); break;
            case CMD_LOAD_DATA : addTask(new MktDataTask(filepath2prefix, taskDao, stocksService)); break;
            default : logger.error("no such cmd in task manager.");
        }
    }

    public void addTask(TaskBase task) {
    	taskExecutor.execute(task);
    }

}
