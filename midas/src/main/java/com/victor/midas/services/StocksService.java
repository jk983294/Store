package com.victor.midas.services;

import javax.ws.rs.*;
import javax.ws.rs.core.*;

import org.springframework.beans.factory.annotation.Autowired;

import com.victor.midas.dao.StockDao;
import com.victor.midas.model.*;

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
