package com.victor.midas.calculator;

import com.victor.midas.model.db.IndexDb;
import com.victor.midas.model.vo.StockVo;
import com.victor.midas.util.MidasConstants;

/**
 * interface for index calculation
 */
public abstract class IndexCalcbase {

    public String getIndexNameOfStock(String stockName){
        return stockName +"_"+ getIndexName();
    }

    /**
     * calculate index
     */
    public void calculate(StockVo stock, IndexDb oldIndex){
        int[] date = stock.getIndex(MidasConstants.INDEX_NAME_DATE).getIndexInt();
        int startDate = date[0], endDate = date[date.length - 1];
        IndexDb index = stock.getIndex(getIndexName());
        if(index == null){
            stock.addIndex(calculateFromScratch(stock));
        } else {
            stock.addIndex(calculateFromExisting(stock));
        }
    }

    public abstract String getIndexName();

    protected abstract IndexDb calculateFromScratch(StockVo stock);

    protected abstract IndexDb calculateFromExisting(StockVo stock);





}
