package com.victor.midas.endpoint;

import com.victor.midas.dao.StockInfoDao;
import com.victor.midas.dao.TaskDao;
import com.victor.midas.model.db.StockInfoDb;
import com.victor.midas.model.vo.StockVo;
import com.victor.midas.services.StocksService;
import com.victor.midas.services.TypeAhead;
import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;

import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;
import java.util.List;

@Path("stocks")
public class StocksEndpoint {
	private final Logger logger = Logger.getLogger(StocksEndpoint.class);
	
	@Autowired
	private StockInfoDao stockInfoDao;
	@Autowired
	private TypeAhead typeAhead;
    @Autowired
    private StocksService stocksService;
	
	@GET
	@Path("/stock/{name}")
	@Produces(MediaType.APPLICATION_JSON)
	public StockVo getStock(@PathParam("name") String name) {
		return stocksService.getStockWithAllIndex(name);
	}

    @GET
    @Path("/")
    @Produces(MediaType.APPLICATION_JSON)
    public List<StockInfoDb> getStockBasicInfo() {
        return stockInfoDao.queryAllBasicInfo();
    }
	
	@GET
	@Path("/typeahead/{query}")
	@Produces(MediaType.APPLICATION_JSON)
	public List<String> getTips(@PathParam("query") String query) {
		List<String> tips = typeAhead.query(query);		
		return tips;
	}
	

	
	@GET
	@Path("/test/")
	public String test() {
		logger.info(stockInfoDao.getStockCount());
		logger.info("page");
		logger.info(stockInfoDao.getStockByPaging(4, 1));
		logger.info("basic info");
		logger.info(stockInfoDao.queryAllBasicInfo());
		return "OK";
	}
	
	
}
