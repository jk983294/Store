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
	
	@Autowired
    private TaskDao taskDao;

	@RequestMapping(value="/", method= RequestMethod.GET)
	@Produces(MediaType.APPLICATION_JSON)
	public List<TaskDb> getLastestTask() {
		return taskDao.queryLastTasks(5);
	}

	/**
	 * deliver task to delete all stocks in MongoDB
	 * @return
	 */
	@RequestMapping(value="/", method= RequestMethod.DELETE)
	public Response deleteTasks() {	
		taskDao.deleteCollection();
		return Response.ok().build();
	}
	
	/**
	 * create task collection
	 * @return
	 */
	@RequestMapping(value="/", method= RequestMethod.PUT)
	public Response updateTasks() {		
		taskDao.createCollection();
		return Response.ok().build();
	}
}
