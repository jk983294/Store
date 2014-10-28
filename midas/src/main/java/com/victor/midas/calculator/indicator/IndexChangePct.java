package com.victor.midas.calculator.indicator;

import com.victor.midas.calculator.IndexCalcbase;
import com.victor.midas.model.vo.StockVo;
import com.victor.midas.util.MidasConstants;
import com.victor.midas.util.MidasException;
import com.victor.utilities.utils.ArrayHelper;

import java.util.Map;

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
    protected Map<String, Object> calculateFromScratch(StockVo stock) throws MidasException {
        double[] end = (double[])stock.queryCmpIndex(MidasConstants.INDEX_NAME_END);
        int len = end.length;
        double[] changePct = new double[len];
        for (int i = 1; i < len; i++) {
            changePct[i] = (end[i] - end[i-1]) / end[i-1];
        }
        return generateCmpName2IndexData(getIndexName(), changePct);
    }

    @Override
    protected Map<String, Object> calculateFromExisting(StockVo stock, StockVo oldStock) throws MidasException {
        double[] end = (double[])stock.queryCmpIndex(MidasConstants.INDEX_NAME_END);
        int len = end.length;
        double[] oldChangePct = (double[])oldStock.queryCmpIndex(getIndexName());

        double[] changePct = ArrayHelper.copyToNewLenArray(oldChangePct, len);
        for (int i = Math.max(1, oldChangePct.length); i < len; i++) {
            changePct[i] = (end[i] - end[i-1]) / end[i-1];
        }

        return generateCmpName2IndexData(getIndexName(), changePct);
    }
}
