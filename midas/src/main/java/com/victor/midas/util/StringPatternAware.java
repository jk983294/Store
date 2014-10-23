package com.victor.midas.util;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Created by Administrator on 2014/10/23.
 */
public class StringPatternAware {

    public static final String NOT_UNDER_CONCERN = "[^0-9a-zA-Z]";
    public static final String ONLY_NUMBER = "[0-9]{1,6}";
    public static final Pattern onlyNumberPattern = Pattern.compile(ONLY_NUMBER);

    public static final String[] STOCK_PREFIX = {"IDX","SZ","SH"};
    public static String STOCK_CODE_PATTERN = null;        // "(IDX|SZ|SH)[0-9]{6}"
    public static Pattern stockCodePattern = null;
    static {
        StringBuilder sb = new StringBuilder();
        sb.append("(");
        for (int i = 0; i < STOCK_PREFIX.length - 1; i++) {
            sb.append(STOCK_PREFIX[i]).append("|");
        }
        sb.append(STOCK_PREFIX[STOCK_PREFIX.length - 1]).append(")").append("[0-9]{6}");
        STOCK_CODE_PATTERN = sb.toString();
        stockCodePattern = Pattern.compile(STOCK_CODE_PATTERN);
    }

    public static boolean isOnlyNumber(String pattern){
        if(pattern == null || pattern == "") return false;
        else return onlyNumberPattern.matcher(pattern).matches();
    }

    public static boolean isStockCode(String pattern){
        if(pattern == null || pattern == "") return false;
        else return stockCodePattern.matcher(pattern).matches();
    }

}
