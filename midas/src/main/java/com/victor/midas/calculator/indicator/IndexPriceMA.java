package com.victor.midas.calculator.indicator;

import com.victor.midas.calculator.IndexCalcbase;
import com.victor.midas.model.vo.StockVo;
import com.victor.midas.util.MidasConstants;
import com.victor.midas.util.MidasException;
import com.victor.utilities.math.stats.StatisticsCalc;

import java.util.Map;

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
    protected Map<String, Object> calculateFromScratch(StockVo stock) throws MidasException {
        double[] end = (double[])stock.queryCmpIndex(MidasConstants.INDEX_NAME_END);
        double[] ma = StatisticsCalc.mean1(end, interval);
        return generateCmpName2IndexData( getIndexName(), ma);
    }

    @Override
    protected Map<String, Object> calculateFromExisting(StockVo stock, StockVo oldStock) throws MidasException {
        double[] end = (double[])stock.queryCmpIndex(MidasConstants.INDEX_NAME_END);
        double[] oldIndexValue = (double[])oldStock.queryCmpIndex(getIndexName());
        double[] ma = StatisticsCalc.meanFromExisting(end, oldIndexValue, interval);

        return generateCmpName2IndexData( getIndexName(), ma);
    }
}
