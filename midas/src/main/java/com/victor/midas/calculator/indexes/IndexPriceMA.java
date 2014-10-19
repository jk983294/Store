package com.victor.midas.calculator.indexes;

import com.victor.midas.calculator.IndexCalc;
import com.victor.midas.model.db.IndexDb;
import com.victor.midas.model.vo.StockVo;
import com.victor.midas.util.MidasConstants;
import com.victor.utilities.math.stats.StatisticsCalc;

/**
 * calculate Price Moving Average
 */
public class IndexPriceMA implements IndexCalc {

    private static final String INDEX_NAME = "pMA";

    private int interval;

    private String indexName;

    public IndexPriceMA(int interval) {
        this.interval = interval;
        this.indexName = INDEX_NAME + interval;
    }

    @Override
    public String getIndexName() {
        return indexName;
    }

    @Override
    public String getIndexNameOfStock(String stockName) {
        return null;
    }

    @Override
    public IndexDb calculate(StockVo stock) {
        double[] end = stock.getIndex(MidasConstants.INDEX_NAME_END).getIndexDouble();
        double[] ma = StatisticsCalc.mean1(end, interval);
        return new IndexDb(stock.getStockName(), getIndexName(), ma);
    }
}
