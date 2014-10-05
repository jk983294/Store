package com.victor.midas.model.db.misc;

import java.util.List;

/**
 * contain a list of stock names
 */
public class StockNamesDb extends MiscBase {

    private List<String> stockNames;

    public StockNamesDb(String miscName, List<String> stockNames) {
        super(miscName);
        this.stockNames = stockNames;
    }

    public List<String> getStockNames() {
        return stockNames;
    }

    public void setStockNames(List<String> stockNames) {
        this.stockNames = stockNames;
    }

    @Override
    public String toString() {
        return "StockNamesDb{" +
                "stockNames=" + stockNames +
                '}';
    }
}
