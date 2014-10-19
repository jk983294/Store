package com.victor.midas.calculator;

import com.victor.midas.model.db.IndexDb;
import com.victor.midas.model.vo.StockVo;

/**
 * interface for index calculation
 */
public interface  IndexCalc {

    String getIndexName();

    String getIndexNameOfStock(String stockName);

    /**
     * calculate index
     */
    IndexDb calculate(StockVo stock);
}
