package com.victor.midas.model.vo;


import com.victor.midas.util.MidasException;
import com.victor.utilities.utils.DateHelper;
import org.springframework.data.annotation.Id;

import java.text.ParseException;
import java.util.*;

/**
 * stock value object
 */
public class StockVo {
    @Id
    private String stockName;

    private String desp;

    /*** int represented times */
    private int start;

    private int end;

    /*** date represented times */
    private Date startDate;

    private Date endDate;

    /*** times series */
    private int[] datesInt;

    private Date[] datesDate;

    /*** index Component Name -> indexData*/
    private Map<String, int[]> indexInts;

    private Map<String, double[]> indexDoubles;
    /*** index Name -> index Component Names List*/
    private Map<String, List<String>> indexName2indexCmp;

    public StockVo(String stockName, String desp) {
        this.stockName = stockName;
        this.desp = desp;
        indexInts = new HashMap<>();
        indexDoubles = new HashMap<>();
        indexName2indexCmp = new HashMap<>();
    }

    /**
     * add times series
     */
    public void addTimeSeries( int[] times) throws ParseException {
        datesInt = times;
        start = datesInt[0];
        end = datesInt[datesInt.length - 1];
        datesDate = DateHelper.toDates(times);
        startDate = datesDate[0];
        endDate = datesDate[datesDate.length - 1];
    }

    /**
     * default indexName map to indexData
     */
    public void addIndex(String indexName, int[] indexData){
        indexInts.put(indexName, indexData);
        indexName2indexCmp.put(indexName, getindexDefaultCmpList(indexName));
    }

    public void addIndex(String indexName, double[] indexData){
        indexDoubles.put(indexName, indexData);
        indexName2indexCmp.put(indexName, getindexDefaultCmpList(indexName));
    }

    /**
     * indexData is indexName map to ( cmpName map to double[] or int[])
     */
    public void addIndex(String indexName, Map<String, Object> indexData) throws MidasException {
        for(Map.Entry<String, Object> entry : indexData.entrySet()){
            String cmpName = entry.getKey();
            if(entry.getValue() instanceof double[]){
                indexDoubles.put(cmpName, (double[])entry.getValue());
            } else if(entry.getValue() instanceof int[]){
                indexInts.put(cmpName, (int[])entry.getValue());
            } else {
                throw new MidasException("index type not compatible");
            }
        }
        indexName2indexCmp.put(indexName, new ArrayList<String>(indexData.keySet()));
    }

    /**
     * indexName map to ( cmpName map to double[] or int[])
     */
    public void addIndex(Map<String, Map<String, Object>> indexData) throws MidasException {
        for(Map.Entry<String, Map<String, Object>> entry : indexData.entrySet()){
            addIndex(entry.getKey(), entry.getValue());
        }
    }

    /**
     * default map from index Name -> index Component Names List
     * ( index Name ) == ( index Component Names List )
     */
    private List<String> getindexDefaultCmpList(String indexName){
        List<String> indexCmpList = new ArrayList<>();
        indexCmpList.add(indexName);
        return indexCmpList;
    }

    /**
     * query data
     */
    public double queryLastIndexDouble(String indexName) throws MidasException {
        double[] data = indexDoubles.get(indexName);
        if(data == null){
            throw new MidasException("no such cmp index : " + indexName);
        }
        return data[data.length - 1];
    }

    public int queryLastIndexInt(String indexName) throws MidasException {
        int[] data = indexInts.get(indexName);
        if(data == null){
            throw new MidasException("no such cmp index : " + indexName);
        }
        return data[data.length - 1];
    }

    public Object queryCmpIndex(String cmpName) throws MidasException {
        if(indexInts != null && indexInts.containsKey(cmpName)){
            return indexInts.get(cmpName);
        } else if(indexDoubles != null && indexDoubles.containsKey(cmpName)){
            return indexDoubles.get(cmpName);
        } else {
            throw new MidasException("no cmp index for " + stockName + " query " + cmpName);
        }
    }

    public boolean isExistIndex(String indexName){
        return indexName2indexCmp.containsKey(indexName);
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

    public Map<String, int[]> getIndexInts() {
        return indexInts;
    }

    public void setIndexInts(Map<String, int[]> indexInts) {
        this.indexInts = indexInts;
    }

    public Map<String, double[]> getIndexDoubles() {
        return indexDoubles;
    }

    public void setIndexDoubles(Map<String, double[]> indexDoubles) {
        this.indexDoubles = indexDoubles;
    }

    public Map<String, List<String>> getIndexName2indexCmp() {
        return indexName2indexCmp;
    }

    public void setIndexName2indexCmp(Map<String, List<String>> indexName2indexCmp) {
        this.indexName2indexCmp = indexName2indexCmp;
    }

    public int getStart() {
        return start;
    }

    public void setStart(int start) {
        this.start = start;
    }

    public int getEnd() {
        return end;
    }

    public void setEnd(int end) {
        this.end = end;
    }

    public Date getStartDate() {
        return startDate;
    }

    public void setStartDate(Date startDate) {
        this.startDate = startDate;
    }

    public Date getEndDate() {
        return endDate;
    }

    public void setEndDate(Date endDate) {
        this.endDate = endDate;
    }

    public int[] getDatesInt() {
        return datesInt;
    }

    public void setDatesInt(int[] datesInt) {
        this.datesInt = datesInt;
    }

    public Date[] getDatesDate() {
        return datesDate;
    }

    public void setDatesDate(Date[] datesDate) {
        this.datesDate = datesDate;
    }

    @Override
    public String toString() {
        return "StockVo{" +
                "stockName='" + stockName + '\'' +
                ", desp='" + desp + '\'' +
                ", start=" + start +
                ", end=" + end +
                ", startDate=" + startDate +
                ", endDate=" + endDate +
                ", datesInt=" + Arrays.toString(datesInt) +
                ", datesDate=" + Arrays.toString(datesDate) +
                ", indexInts=" + indexInts +
                ", indexDoubles=" + indexDoubles +
                ", indexName2indexCmp=" + indexName2indexCmp +
                '}';
    }
}
