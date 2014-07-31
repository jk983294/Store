package com.victor.midas.services;

import javax.ws.rs.*;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

import org.springframework.beans.factory.annotation.Autowired;

import com.victor.midas.dao.StockDao;
import com.victor.midas.model.*;
import com.victor.midas.worker.MktDataLoader;

@Path("stocks")
public class StocksService {
	@Autowired
	StockDao stockdao;
	
	@GET
	@Path("/stock/{name}")
	@Produces(MediaType.APPLICATION_JSON)
	public Stock getStock(@PathParam("name") String name) {
		Stock stock = stockdao.queryByName(name);		
		return stock;
	}
	
	
}
