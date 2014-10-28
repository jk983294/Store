package com.victor.midas.services;

import java.util.List;

import com.victor.midas.dao.StockDao;
import com.victor.midas.dao.MiscDao;
import com.victor.midas.dao.StockInfoDao;
import com.victor.midas.dao.TaskDao;
import com.victor.midas.model.db.StockInfoDb;
import com.victor.midas.model.db.misc.StockNamesDb;
import com.victor.midas.model.vo.StockVo;

import com.victor.midas.util.MidasException;
import com.victor.midas.util.ModelConvertor;
import org.apache.log4j.Logger;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

@Component
public class StocksService {
	private final Logger logger = Logger.getLogger(StocksService.class);

    @Autowired
    private StockInfoDao stockInfoDao;
    @Autowired
    private TaskDao taskDao;
    @Autowired
    private StockDao stockDao;
    @Autowired
    private MiscDao miscDao;

	@Autowired
	private TypeAhead typeAhead;


    public void saveStocks(List<StockVo> stocks) throws MidasException {
        StockNamesDb stockNames = ModelConvertor.convert2StockNames(stocks);
        miscDao.saveMisc(stockNames);

        List<StockInfoDb> stockInfoDbs = ModelConvertor.convert2StockInfo(stocks);
        stockInfoDao.saveStockInfo(stockInfoDbs);
        stockDao.saveStock(stocks);
    }

    public StockVo getStockWithAllIndex(String stockName){
        return stockDao.queryStock(stockName);
    }


    public StockInfoDao getStockInfoDao() {
        return stockInfoDao;
    }

    public TaskDao getTaskDao() {
        return taskDao;
    }

    public StockDao getStockDao() {
        return stockDao;
    }

    public MiscDao getMiscDao() {
        return miscDao;
    }

    public TypeAhead getTypeAhead() {
        return typeAhead;
    }
}
