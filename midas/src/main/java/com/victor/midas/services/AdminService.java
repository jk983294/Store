package com.victor.midas.services;

import javax.ws.rs.*;
import javax.ws.rs.core.*;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;

import com.victor.midas.worker.common.TaskMgr;

@Path("admin")
public class AdminService {
	
	
	@Autowired
	private TaskMgr taskMgr;
	
	private static final Logger logger = Logger.getLogger(AdminService.class);
	
	@PUT
	@Path("/stocks")
	public Response updateStocks() {		
		taskMgr.cmd( "updateStocks" );
		return Response.ok().build();
	}
	
	/**
	 * deliver task to delete all stocks in MongoDB
	 * @return
	 */
	@DELETE
	@Path("/stocks")
	public Response deleteStocks() {	
		taskMgr.cmd( "deleteStocks" );
		return Response.ok().build();
	}
	
	
	
}
