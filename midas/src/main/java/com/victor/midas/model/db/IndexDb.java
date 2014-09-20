package com.victor.midas.model.db;

import org.springframework.data.annotation.Id;

/**
 * index for a stock
 */
public class IndexDb {
    @Id
    private String name;

    private String indexName;

    private int startDate;
    private int endDate;
    private double[] index;
    private int[] date;

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getIndexName() {
        return indexName;
    }

    public void setIndexName(String indexName) {
        this.indexName = indexName;
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

    public double[] getIndex() {
        return index;
    }

    public void setIndex(double[] index) {
        this.index = index;
    }

    public int[] getDate() {
        return date;
    }

    public void setDate(int[] date) {
        this.date = date;
    }
}
