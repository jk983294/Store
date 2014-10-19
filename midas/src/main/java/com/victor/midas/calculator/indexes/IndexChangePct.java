package com.victor.midas.calculator.indexes;

import com.victor.midas.calculator.IndexCalc;
import com.victor.midas.model.db.IndexDb;
import com.victor.midas.model.vo.StockVo;
import com.victor.midas.util.MidasConstants;

/**
 * calculate change percentage per day
 */
public class IndexChangePct implements IndexCalc {

    private static final String INDEX_NAME = "changePct";

    @Override
    public String getIndexName() {
        return INDEX_NAME;
    }

    @Override
    public String getIndexNameOfStock(String stockName) {
        return null;
    }

    @Override
    public IndexDb calculate(StockVo stock) {
        double[] end = stock.getIndex(MidasConstants.INDEX_NAME_END).getIndexDouble();
        int len = end.length;
        double[] changePct = new double[len];
        for (int i = 1; i < len; i++) {
            changePct[i] = (end[i] - end[i-1]) / end[i-1];
        }
        return new IndexDb(stock.getStockName(), getIndexName(), changePct);
    }
}
