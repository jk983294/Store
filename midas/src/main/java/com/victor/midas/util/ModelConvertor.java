package com.victor.midas.util;

import com.victor.midas.model.db.*;
import com.victor.midas.model.db.misc.StockNamesDb;
import com.victor.midas.model.vo.StockVo;

import java.util.ArrayList;
import java.util.Date;
import java.util.List;

/**
 * convert models
 */
public class ModelConvertor {

    public static StockNamesDb convert2StockNames(List<StockVo> stocks){
        List<String> stockNames = new ArrayList<>();
        for (StockVo stock : stocks){
            stockNames.add(stock.getStockName());
        }
        return new StockNamesDb(MidasConstants.MISC_ALL_STOCK_NAMES, stockNames);
    }

    public static List<StockInfoDb> convert2StockInfo(List<StockVo> stocks) throws MidasException {
        List<StockInfoDb> stockInfoDbs = new ArrayList<>();
        for (StockVo stock : stocks){
            StockInfoDb stockInfo = new StockInfoDb();
            stockInfo.setName(stock.getStockName());
            stockInfo.setDesp(stock.getDesp());
            stockInfo.setDate(stock.getEnd());
            stockInfo.setStart(stock.getStart());
            stockInfo.setMax(stock.queryLastIndexDouble(MidasConstants.INDEX_NAME_MAX));
            stockInfo.setMin(stock.queryLastIndexDouble(MidasConstants.INDEX_NAME_MIN));
            stockInfo.setEnd(stock.queryLastIndexDouble(MidasConstants.INDEX_NAME_END));
            stockInfo.setVolume(stock.queryLastIndexInt(MidasConstants.INDEX_NAME_VOLUME));
            stockInfo.setTotal(stock.queryLastIndexDouble(MidasConstants.INDEX_NAME_TOTAL));
            stockInfo.setChange(stock.queryLastIndexDouble(MidasConstants.INDEX_NAME_CHANGEPCT));

            stockInfoDbs.add(stockInfo);
        }
        return stockInfoDbs;
    }

}
