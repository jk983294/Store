package com.victor.midas.calculator;

import java.util.ArrayList;
import java.util.List;


import com.victor.midas.dao.StockDao;
import com.victor.midas.model.vo.StockVo;
import com.victor.midas.calculator.indicator.*;
import com.victor.midas.util.MidasException;
import org.apache.log4j.Logger;

public class IndexCalculator {
    private static final Logger logger = Logger.getLogger(IndexCalculator.class);

    private static List<IndexCalcbase> indexCalcbases = new ArrayList<>();
    private static List<String> indexNames = new ArrayList<>();

    static {
        /*** prepare all index calculator*/
        indexCalcbases.add(new IndexChangePct());

        int[] PRICE_MA_INTERVALS = new int[]{ 5, 15, 30};
        for( int interval : PRICE_MA_INTERVALS){
            indexCalcbases.add(new IndexPriceMA(interval));
        }

        /*** store all index names*/
        for (IndexCalcbase indexCalcbase : indexCalcbases){
            indexNames.add(indexCalcbase.getIndexName());
        }
    }

    private List<StockVo> stocks;

    private StockDao stockDao;

    public IndexCalculator(List<StockVo> stocks, StockDao stockDao) {
        this.stocks = stocks;
        this.stockDao = stockDao;
    }

    public void calculate() throws MidasException {
        logger.info("calculation use incremental mode : " + IndexCalcbase.useExistingData);
		for(StockVo stock : stocks){
            try {
                StockVo oldStock = stockDao.queryStock(stock.getStockName());
                for (IndexCalcbase indexCalcbase : indexCalcbases){
                    indexCalcbase.calculate(stock, oldStock);
                }
            } catch (Exception e){
                logger.error(e);
                throw new MidasException("problem meet when calculate index for " + stock, e);
            }

        }
	}

}
