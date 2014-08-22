package com.victor.midas.services;

import java.util.List;

import javax.ws.rs.*;
import javax.ws.rs.core.*;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;

import com.victor.midas.dao.StockDao;
import com.victor.midas.model.*;

@Path("stocks")
public class StocksService {
	@Autowired
	StockDao stockDao;
	
	private final Logger logger = Logger.getLogger(StocksService.class);
	
	@GET
	@Path("/stock/{name}")
	@Produces(MediaType.APPLICATION_JSON)
	public Stock getStock(@PathParam("name") String name) {
		Stock stock = stockDao.queryByName(name);		
		return stock;
	}
	
	@GET
	@Path("/")
	@Produces(MediaType.APPLICATION_JSON)
	public List<Stock> getStockBasicInfo() {
		return stockDao.queryAllBasicInfo();
	}
	
	@GET
	@Path("/test/")
	public String test() {
		logger.info(stockDao.getStockCount());
		logger.info("page");
		logger.info(stockDao.getStockByPaging(4, 1));
		logger.info("basic info");
		logger.info(stockDao.queryAllBasicInfo());
		return "OK";
	}
	
	
}
