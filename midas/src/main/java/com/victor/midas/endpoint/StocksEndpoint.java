package com.victor.midas.endpoint;

import com.victor.midas.dao.StockInfoDao;
import com.victor.midas.dao.TaskDao;
import com.victor.midas.model.db.StockInfoDb;
import com.victor.midas.model.vo.StockVo;
import com.victor.midas.services.StocksService;
import com.victor.midas.services.TypeAhead;
import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;
import java.util.ArrayList;
import java.util.List;


@RestController
@RequestMapping("/stocks")
public class StocksEndpoint {
	private final Logger logger = Logger.getLogger(StocksEndpoint.class);
	
	@Autowired
	private StockInfoDao stockInfoDao;
    @Autowired
    private StocksService stocksService;
	
	@GET
    @RequestMapping("/{name}")
	@Produces(MediaType.APPLICATION_JSON)
	public StockVo getStock(@PathVariable("name") String name) {
		return stocksService.getStockWithAllIndex(name);
	}

    @GET
    @RequestMapping("/{name1}/{name2}")
    @Produces(MediaType.APPLICATION_JSON)
    public List<StockVo> getTwoStocks(@PathVariable("name1") String name1, @PathVariable("name2") String name2) {
        List<StockVo> array = new ArrayList<>();
        array.add(stocksService.getStockWithAllIndex(name1));
        array.add(stocksService.getStockWithAllIndex(name2));
        return array;
    }

    @GET
    @RequestMapping("/stockinfos")
    @Produces(MediaType.APPLICATION_JSON)
    public List<StockInfoDb> getStockBasicInfo() {
        return stockInfoDao.queryAllBasicInfo();
    }
	

	

	
	@GET
	@RequestMapping("/test/")
	public String test() {
		logger.info(stockInfoDao.getStockCount());
		logger.info("page");
		logger.info(stockInfoDao.getStockByPaging(4, 1));
		logger.info("basic info");
		logger.info(stockInfoDao.queryAllBasicInfo());
		return "OK";
	}
	
	
}
