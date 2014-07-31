package com.victor.midas.services;

import javax.ws.rs.*;
import javax.ws.rs.core.*;
import javax.ws.rs.core.Response.*;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;

import com.victor.midas.dao.StockDao;
import com.victor.midas.model.*;
import com.victor.midas.worker.MktDataLoader;

@Path("admin")
public class AdminService {
	@Autowired
	StockDao stockdao;
	
	@Autowired
	MktDataLoader mktloader;
	
	private final Logger logger = Logger.getLogger(HelloRest.class);
	
	@PUT
	@Path("/stocks")
	public Response updateStocks() {		
		try {
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
		stockdao.dropCollection();
		return Response.ok().build();
	}
	
	
}
