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
    private MidasConstants.IndexType indexType;

    private double[] indexDouble;
    private int[] indexInt;

    private int startDate;
    private int endDate;

    public IndexDb(String stockName, String indexName, double[] indexDouble, int[] date) {
        this.stockName = stockName;
        this.indexName = indexName;
        this.indexDouble = indexDouble;
        indexType = MidasConstants.IndexType.DOUBLE;
        this.id = MidasConstants.getIndexId(stockName, indexName);
        if(date != null && date.length > 0){
            startDate = date[0];
            endDate = date[date.length - 1];
        }
    }


    public IndexDb(String stockName, String indexName, int[] indexInt, int[] date) {
        this.stockName = stockName;
        this.indexName = indexName;
        this.indexInt = indexInt;
        indexType = MidasConstants.IndexType.INT;
        this.id = MidasConstants.getIndexId(stockName, indexName);
        if(date != null && date.length > 0){
            startDate = date[0];
            endDate = date[date.length - 1];
        }
    }

    public IndexDb(String stockName, String indexName, double[] indexDouble, int startDate, int endDate) {
        this.stockName = stockName;
        this.indexName = indexName;
        this.indexDouble = indexDouble;
        indexType = MidasConstants.IndexType.DOUBLE;
        this.id = MidasConstants.getIndexId(stockName, indexName);
        this.startDate = startDate;
        this.endDate = endDate;
    }


    public IndexDb(String stockName, String indexName, int[] indexInt, int startDate, int endDate) {
        this.stockName = stockName;
        this.indexName = indexName;
        this.indexInt = indexInt;
        indexType = MidasConstants.IndexType.INT;
        this.id = MidasConstants.getIndexId(stockName, indexName);
        this.startDate = startDate;
        this.endDate = endDate;
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

    public MidasConstants.IndexType getIndexType() {
        return indexType;
    }

    public void setIndexType(MidasConstants.IndexType indexType) {
        this.indexType = indexType;
    }

    public int getStartDate() {
        return startDate;
    }

    public void setStartDate(int startDate) {
        this.startDate = startDate;
    }

    public int getEndDate() {
        return endDate;
    }

    public void setEndDate(int endDate) {
        this.endDate = endDate;
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
