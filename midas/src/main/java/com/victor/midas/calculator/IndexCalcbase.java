package com.victor.midas.calculator;

import com.victor.midas.model.db.IndexDb;
import com.victor.midas.model.vo.StockVo;
import com.victor.midas.util.MidasConstants;
import org.apache.log4j.Logger;

/**
 * interface for index calculation
 */
public abstract class IndexCalcbase {

    private static final Logger logger = Logger.getLogger(IndexCalcbase.class);

    private static final boolean useExistingData = false;

    public String getIndexNameOfStock(String stockName){
        return stockName +"_"+ getIndexName();
    }

    public abstract String getIndexName();

    /**
     * no old result value to use, calculate it from scratch
     */
    protected abstract IndexDb calculateFromScratch(StockVo stock);

    /**
     * use oldIndex value, reduce calculation overhead
     */
    protected abstract IndexDb calculateFromExisting(StockVo stock, IndexDb oldIndex);

    /**
     * calculate index
     */
    public void calculate(StockVo stock, IndexDb oldIndex){
        int[] date = stock.getIndex(MidasConstants.INDEX_NAME_DATE).getIndexInt();
        int startDate = date[0];
        IndexDb index = stock.getIndex(getIndexName());
        if( !useExistingData || index == null || oldIndex == null || oldIndex.getStartDate() != startDate){
            stock.addIndex(calculateFromScratch(stock));
        } else if(date[date.length - 1] != oldIndex.getEndDate()){
            logger.info("calculateFromExisting for stock : " + stock.getStockName() + " index : " + getIndexName());
            stock.addIndex(calculateFromExisting(stock, oldIndex));
        } else {
            logger.info("already calculate index for stock : " + stock.getStockName() + " index : " + getIndexName());
        }
    }

}
