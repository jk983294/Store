package com.victor.midas.util;

import com.victor.utilities.math.utils.MathHelper;

import java.util.ArrayList;
import java.util.List;

/**
 * constant variable lives here
 */
public class MidasConstants {

    /**
     * misc collection MiscName for MongoDB index
     */
    public static final String MISC_ALL_STOCK_NAMES = "AllStockNames";

    /**
     * collection name
     */
    public static final String INDEX_COLLECTION_NAME = "StockIndex";
    public static final String MISC_COLLECTION_NAME = "StockMisc";
    public static final String STOCKINFO_COLLECTION_NAME = "StockInfo";
    public static final String TASK_COLLECTION_NAME = "StockTask";

    /**
     * index name
     */
    public static final String INDEX_NAME_DATE = "date";
    public static final String INDEX_NAME_START = "start";
    public static final String INDEX_NAME_MAX = "max";
    public static final String INDEX_NAME_MIN = "min";
    public static final String INDEX_NAME_END = "end";
    public static final String INDEX_NAME_VOLUME = "volume";
    public static final String INDEX_NAME_TOTAL = "total";
    public static final String INDEX_NAME_CHANGEPCT = "changePct";

    public static String getIndexId(String stockName, String indexName){
        StringBuilder sb = new StringBuilder(stockName);
        sb.append("_").append(indexName);
        return sb.toString();
    }

    /**
     * typeahead actions
     */
    public static List<String> actions;

    static {
        String[] actionArray={"cmp","task"};
        actions = MathHelper.array2list(actionArray);
    }
}
