package com.victor.midas.model.db;

import com.victor.midas.util.MidasConstants;
import org.springframework.data.annotation.Id;

import java.util.Arrays;

/**
 * index for a stock
 */
public class IndexDb {
    @Id
    private String id;  // id = "stockName_indexName"

    private String stockName;
    private String indexName;

    private double[] indexDouble;
    private int[] indexInt;

    public IndexDb(String stockName, String indexName, double[] indexDouble, int[] indexInt) {
        this.stockName = stockName;
        this.indexName = indexName;
        this.indexDouble = indexDouble;
        this.indexInt = indexInt;
        this.id = MidasConstants.getIndexId(stockName, indexName);
    }

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getStockName() {
        return stockName;
    }

    public void setStockName(String stockName) {
        this.stockName = stockName;
    }

    public String getIndexName() {
        return indexName;
    }

    public void setIndexName(String indexName) {
        this.indexName = indexName;
    }

    public double[] getIndexDouble() {
        return indexDouble;
    }

    public void setIndexDouble(double[] indexDouble) {
        this.indexDouble = indexDouble;
    }

    public int[] getIndexInt() {
        return indexInt;
    }

    public void setIndexInt(int[] indexInt) {
        this.indexInt = indexInt;
    }

    @Override
    public String toString() {
        return "IndexDb{" +
                "id='" + id + '\'' +
                ", stockName='" + stockName + '\'' +
                ", indexName='" + indexName + '\'' +
                ", indexDouble=" + Arrays.toString(indexDouble) +
                ", indexInt=" + Arrays.toString(indexInt) +
                '}';
    }
}
