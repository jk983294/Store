package com.victor.midas.calculator;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;


import com.victor.midas.dao.IndexDao;
import com.victor.midas.model.db.IndexDb;
import com.victor.midas.model.vo.StockVo;
import com.victor.midas.calculator.indicator.*;
import com.victor.midas.services.StocksService;
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

    private IndexDao indexDao;

    public IndexCalculator(List<StockVo> stocks, IndexDao indexDao) {
        this.stocks = stocks;
        this.indexDao = indexDao;
    }

    public void calculate() throws IndexCalcException {
        logger.info("calculation use incremental mode : " + IndexCalcbase.useExistingData);
		for(StockVo stock : stocks){
            try {
                List<IndexDb> oldIndexes = indexDao.queryAllIndex(stock.getStockName());
                Map<String, IndexDb> indexName2OldIndexdb = convertList2Map(oldIndexes);
                for (IndexCalcbase indexCalcbase : indexCalcbases){
                    IndexDb oldIndex = indexName2OldIndexdb.get(indexCalcbase.getIndexNameOfStock(stock.getStockName()));
                    indexCalcbase.calculate(stock, oldIndex);
                }
            } catch (Exception e){
                logger.error(e);
                throw new IndexCalcException("problem meet when calculate index for " + stock, e);
            }

        }
	}

    /**
     * query all indexDb once, reduce db disk query time, use map for query one specified
     */
    private Map<String, IndexDb> convertList2Map(List<IndexDb> oldIndexes){
        Map<String, IndexDb> indexName2OldIndexdb = new HashMap<>();
        for (IndexDb indexDb : oldIndexes){
            indexName2OldIndexdb.put(indexDb.getIndexName(), indexDb);
        }
        return indexName2OldIndexdb;
    }




}
