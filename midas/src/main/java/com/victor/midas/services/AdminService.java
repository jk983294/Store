package com.victor.midas.services;

import javax.ws.rs.*;
import javax.ws.rs.core.*;
import javax.ws.rs.core.Response.*;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;

import com.victor.midas.dao.*;
import com.victor.midas.worker.MktDataLoader;

@Path("admin")
public class AdminService {
	@Autowired
	StockDao stockdao;
	@Autowired
	AdminDao admindao;
	
	@Autowired
	MktDataLoader mktloader;
	
	private final Logger logger = Logger.getLogger(AdminService.class);
	
	@PUT
	@Path("/stocks")
	public Response updateStocks() {		
		try {
			admindao.createCollection();
			mktloader.saveAllFromStockDirPath();
		} catch (Exception e) {
			logger.error("update stocks got problems : "+e.toString());
			return Response.status(Status.INTERNAL_SERVER_ERROR).build();
		}
		return Response.ok().build();
	}
	
	@DELETE
	@Path("/stocks")
	public Response deleteStocks() {	
		admindao.dropStockCollection();
		return Response.ok().build();
	}
	
	
	
}
