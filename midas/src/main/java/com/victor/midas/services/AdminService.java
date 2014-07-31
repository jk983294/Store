package com.victor.midas.services;

import javax.ws.rs.*;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

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
	
	@PUT
	@Path("/stocks")
	public Response updateStocks() {		
		try {
			mktloader.saveAllFromStockDirPath();
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
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
