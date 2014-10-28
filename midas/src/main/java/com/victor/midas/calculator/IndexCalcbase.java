package com.victor.midas.calculator;

import com.victor.midas.model.vo.StockVo;
import com.victor.midas.util.MidasException;
import org.apache.log4j.Logger;

import java.util.HashMap;
import java.util.Map;

/**
 * interface for index calculation
 */
public abstract class IndexCalcbase {

    private static final Logger logger = Logger.getLogger(IndexCalcbase.class);

    public static final boolean useExistingData = false;

    public String getIndexNameOfStock(String stockName){
        return stockName +"_"+ getIndexName();
    }

    public abstract String getIndexName();

    /**
     * no old result value to use, calculate it from scratch
     * return ( cmpName map to double[] or int[])
     */
    protected abstract Map<String, Object> calculateFromScratch(StockVo stock) throws MidasException;

    /**
     * use oldIndex value, reduce calculation overhead
     * return ( cmpName map to double[] or int[])
     */
    protected abstract Map<String, Object> calculateFromExisting(StockVo stock, StockVo oldStock) throws MidasException;

    /**
     * calculate index
     */
    public void calculate(StockVo stock, StockVo oldStock) throws MidasException {
        if( !useExistingData || oldStock == null || !oldStock.isExistIndex(getIndexName()) || oldStock.getStart() != stock.getStart()){
            stock.addIndex(getIndexName(), calculateFromScratch(stock));
        } else if(oldStock.getEnd() != stock.getEnd()){
            logger.info("calculateFromExisting for stock : " + stock.getStockName() + " index : " + getIndexName());
            stock.addIndex(getIndexName(), calculateFromExisting(stock, oldStock));
        } else {
            logger.info("already calculate index for stock : " + stock.getStockName() + " index : " + getIndexName());
        }
    }

    protected Map<String, Object> generateCmpName2IndexData(String cmpName, double[] indexData){
        Map<String, Object> map = new HashMap<>();
        map.put(cmpName, indexData);
        return map;
    }

    protected Map<String, Object> generateCmpName2IndexData(String cmpName, int[] indexData){
        Map<String, Object> map = new HashMap<>();
        map.put(cmpName, indexData);
        return map;
    }

    protected void generateCmpName2IndexData(Map<String, Object> map, String cmpName, int[] indexData){
        map.put(cmpName, indexData);
    }

    protected void generateCmpName2IndexData(Map<String, Object> map, String cmpName, double[] indexData){
        map.put(cmpName, indexData);
    }

}
