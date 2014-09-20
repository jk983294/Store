package com.victor.midas.services;

import java.util.List;

import javax.ws.rs.DELETE;
import javax.ws.rs.GET;
import javax.ws.rs.PUT;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;

import com.victor.midas.dao.TaskDao;
import com.victor.midas.model.db.TaskDb;

@Path("task")
public class TaskService {
	private static final Logger logger = Logger.getLogger(TaskService.class);
	
	@Autowired
    private TaskDao taskDao;
	
	@GET
	@Path("/")
	@Produces(MediaType.APPLICATION_JSON)
	public List<TaskDb> getLastestTask() {
		return taskDao.lastTasks(2);
	}

	/**
	 * deliver task to delete all stocks in MongoDB
	 * @return
	 */
	@DELETE
	@Path("/")
	public Response deleteTasks() {	
		taskDao.dropTaskCollection();
		return Response.ok().build();
	}
	
	/**
	 * create task collection
	 * @return
	 */
	@PUT
	@Path("/")
	public Response updateTasks() {		
		taskDao.createCollection();
		return Response.ok().build();
	}
}
