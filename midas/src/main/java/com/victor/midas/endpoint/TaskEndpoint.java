package com.victor.midas.endpoint;

import java.util.List;

import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;

import com.victor.midas.dao.TaskDao;
import com.victor.midas.model.db.TaskDb;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;

@RestController
@RequestMapping("task")
public class TaskEndpoint {
	private static final Logger logger = Logger.getLogger(TaskEndpoint.class);

    private static final int QUERY_MAX_TASK_CNT = 10;
	
	@Autowired
    private TaskDao taskDao;

	@RequestMapping(value="/alltasks", method= RequestMethod.GET)
	@Produces(MediaType.APPLICATION_JSON)
	public List<TaskDb> getLastestTask() {
        logger.info("query last " + QUERY_MAX_TASK_CNT + " tasks.");
		return taskDao.queryLastTasks(QUERY_MAX_TASK_CNT);
	}
}
