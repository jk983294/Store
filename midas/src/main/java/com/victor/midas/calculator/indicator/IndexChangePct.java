package com.victor.midas.calculator.indicator;

import com.victor.midas.calculator.IndexCalcbase;
import com.victor.midas.model.db.IndexDb;
import com.victor.midas.model.vo.StockVo;
import com.victor.midas.util.MidasConstants;
import com.victor.utilities.math.utils.ArrayHelper;

/**
 * calculate change percentage per day
 */
public class IndexChangePct extends IndexCalcbase {

    private static final String INDEX_NAME = "changePct";

    @Override
    public String getIndexName() {
        return INDEX_NAME;
    }

    @Override
    protected IndexDb calculateFromScratch(StockVo stock) {
        double[] end = stock.getIndex(MidasConstants.INDEX_NAME_END).getIndexDouble();
        int len = end.length;
        double[] changePct = new double[len];
        for (int i = 1; i < len; i++) {
            changePct[i] = (end[i] - end[i-1]) / end[i-1];
        }

        int[] date = stock.getIndex(MidasConstants.INDEX_NAME_DATE).getIndexInt();
        return new IndexDb(stock.getStockName(), getIndexName(), changePct, date);
    }

    @Override
    protected IndexDb calculateFromExisting(StockVo stock, IndexDb oldIndex) {
        double[] end = stock.getIndex(MidasConstants.INDEX_NAME_END).getIndexDouble();
        int len = end.length;
        double[] oldChangePct = oldIndex.getIndexDouble();

        double[] changePct = ArrayHelper.copyToNewLenArray(oldChangePct, len);
        for (int i = Math.max(1, oldChangePct.length); i < len; i++) {
            changePct[i] = (end[i] - end[i-1]) / end[i-1];
        }

        int[] date = stock.getIndex(MidasConstants.INDEX_NAME_DATE).getIndexInt();
        return new IndexDb(stock.getStockName(), getIndexName(), changePct, date);
    }
}
