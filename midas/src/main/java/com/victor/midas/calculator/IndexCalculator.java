package com.victor.midas.calculator;

import java.util.ArrayList;
import java.util.List;


import com.victor.midas.model.vo.StockVo;
import com.victor.midas.calculator.indexes.*;
import org.apache.log4j.Logger;

public class IndexCalculator {
    private static final Logger logger = Logger.getLogger(IndexCalculator.class);

    private static List<IndexCalc> indexCalcs = new ArrayList<>();
    private static List<String> indexNames = new ArrayList<>();

    static {
        /**
         * prepare all index calculator
         */
        indexCalcs.add(new IndexChangePct());

        int[] PRICE_MA_INTERVALS = new int[]{ 5, 15, 30};
        for( int interval : PRICE_MA_INTERVALS){
            indexCalcs.add(new IndexPriceMA(interval));
        }

        // store all index names
        for (IndexCalc indexCalc : indexCalcs){
            indexNames.add(indexCalc.getIndexName());
        }
    }

    private List<StockVo> stocks;

    public IndexCalculator(List<StockVo> stocks) {
        this.stocks = stocks;
    }

    public void calculate() throws IndexCalcException {
		for(StockVo stock : stocks){
            try {
                for (IndexCalc indexCalc : indexCalcs){
                    stock.addIndex(indexCalc.calculate(stock));
                }
            } catch (Exception e){
                logger.error(e);
                throw new IndexCalcException("problem meet when calculate index for " + stock, e);
            }

        }
	}




}
