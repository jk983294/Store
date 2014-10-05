package com.victor.midas.model.vo;

import com.victor.midas.model.db.IndexDb;

import java.util.HashMap;
import java.util.Map;

/**
 * stock value object
 */
public class StockVo {

    private String stockName;

    private String desp;
    /*** indexName -> indexData*/
    private Map<String, IndexDb> indexes;

    public StockVo(String stockName, String desp) {
        this.stockName = stockName;
        this.desp = desp;
        indexes = new HashMap<>();
    }

    public StockVo(String stockName, String desp, Map<String, IndexDb> indexes) {
        this.stockName = stockName;
        this.desp = desp;
        this.indexes = indexes;
    }

    public void addIndex(IndexDb indexDb){
        indexes.put(indexDb.getIndexName(), indexDb);
    }

    public void addIndex(String indexName, double data[]){
        IndexDb indexDb = new IndexDb(stockName, indexName, data, null);
        indexes.put(indexName, indexDb);
    }

    public void addIndex(String indexName, int data[]){
        IndexDb indexDb = new IndexDb(stockName, indexName, null, data);
        indexes.put(indexName, indexDb);
    }

    public double queryLastIndexDouble(String indexName){
        double[] data = indexes.get(indexName).getIndexDouble();
        return data[data.length - 1];
    }

    public int queryLastIndexInt(String indexName){
        int[] data = indexes.get(indexName).getIndexInt();
        return data[data.length - 1];
    }

    public IndexDb getIndex(String indexName){
        return indexes.get(indexName);
    }

    public String getStockName() {
        return stockName;
    }

    public void setStockName(String stockName) {
        this.stockName = stockName;
    }

    public String getDesp() {
        return desp;
    }

    public void setDesp(String desp) {
        this.desp = desp;
    }

    public Map<String, IndexDb> getIndexes() {
        return indexes;
    }

    public void setIndexes(Map<String, IndexDb> indexes) {
        this.indexes = indexes;
    }

    @Override
    public String toString() {
        return "StockVo{" +
                "stockName='" + stockName + '\'' +
                ", desp='" + desp + '\'' +
                ", indexes=" + indexes +
                '}';
    }
}
