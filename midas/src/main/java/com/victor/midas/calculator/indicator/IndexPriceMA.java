package com.victor.midas.calculator.indicator;

import com.victor.midas.calculator.IndexCalcbase;
import com.victor.midas.model.db.IndexDb;
import com.victor.midas.model.vo.StockVo;
import com.victor.midas.util.MidasConstants;
import com.victor.utilities.math.stats.StatisticsCalc;
import com.victor.utilities.math.utils.ArrayHelper;

/**
 * calculate Price Moving Average
 */
public class IndexPriceMA extends IndexCalcbase {

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
    protected IndexDb calculateFromScratch(StockVo stock) {
        double[] end = stock.getIndex(MidasConstants.INDEX_NAME_END).getIndexDouble();
        double[] ma = StatisticsCalc.mean1(end, interval);

        int[] date = stock.getIndex(MidasConstants.INDEX_NAME_DATE).getIndexInt();
        return new IndexDb(stock.getStockName(), getIndexName(), ma, date);
    }

    @Override
    protected IndexDb calculateFromExisting(StockVo stock, IndexDb oldIndex) {
        double[] end = stock.getIndex(MidasConstants.INDEX_NAME_END).getIndexDouble();
        double[] oldIndexValue = oldIndex.getIndexDouble();
        double[] ma = StatisticsCalc.meanFromExisting(end, oldIndexValue, interval);

        int[] date = stock.getIndex(MidasConstants.INDEX_NAME_DATE).getIndexInt();
        return new IndexDb(stock.getStockName(), getIndexName(), ma, date);
    }
}
