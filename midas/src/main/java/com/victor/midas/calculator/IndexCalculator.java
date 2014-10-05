package com.victor.midas.calculator;

import java.util.List;

import com.victor.midas.model.db.IndexDb;
import com.victor.midas.model.vo.StockVo;
import com.victor.midas.util.MidasConstants;
import com.victor.utilities.math.stats.StatisticsCalc;

public class IndexCalculator {
    private final static String DOUBLE_TYPE = "double";
    private final static String INT_TYPE = "int";
    private static int[] PRICE_MA_INTERVALS = null;

    static {
        PRICE_MA_INTERVALS = new int[]{ 5, 15, 30};
    }

    private List<StockVo> stocks;

    public IndexCalculator(List<StockVo> stocks) {
        this.stocks = stocks;
    }

    public void calculate() {
		for(StockVo stock : stocks){
            if (checkRawDataAvailable(stock)){
                calcChangePct(stock);
                calcPriceMovingAverage(stock);
            }
        }
	}

    /**
     * calculate Price Moving Average by according intervals
     * @param stock
     */
    private void calcPriceMovingAverage(StockVo stock){
        double[] end = stock.getIndex(MidasConstants.INDEX_NAME_END).getIndexDouble();
        for( int interval : PRICE_MA_INTERVALS){
            double[] ma = StatisticsCalc.mean1(end, interval);
            stock.addIndex("pMA" + interval, ma);
        }
    }
	
	/**
	 * calculate change percentage per day
	 */
	private void calcChangePct(StockVo stock){
        double[] end = stock.getIndex(MidasConstants.INDEX_NAME_END).getIndexDouble();
        int len = end.length;
        double[] changePct = new double[len];
        for (int i = 1; i < len; i++) {
            changePct[i] = (end[i] - end[i-1]) / end[i-1];
        }
        stock.addIndex(MidasConstants.INDEX_NAME_CHANGEPCT, changePct);
	}

    /**
     * check the stock index raw data is exists
     */
    private boolean checkRawDataAvailable(StockVo stock){
        if (!checkRawDataAvailable(stock, MidasConstants.INDEX_NAME_END, DOUBLE_TYPE)) return false;
        return true;
    }

    private boolean checkRawDataAvailable(StockVo stock, String indexName, String dataType){
        if (stock == null || stock.getIndexes() == null || stock.getIndexes().get(indexName) == null) {
            return false;
        } else {
            IndexDb indexDb = stock.getIndexes().get(indexName);
            switch ( dataType.toLowerCase() ){
                case DOUBLE_TYPE : {
                    if(indexDb.getIndexDouble() == null) return false;
                } break;
                case INT_TYPE : {
                    if(indexDb.getIndexInt() == null) return false;
                } break;
                default: return false;
            }
        }
        return true;
    }
}
