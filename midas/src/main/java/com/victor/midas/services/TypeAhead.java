package com.victor.midas.services;

import java.util.ArrayList;
import java.util.List;

import org.apache.log4j.Logger;

import com.victor.midas.dao.StockInfoDao;
import com.victor.utilities.datastructures.tree.TernaryTree;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

@Component
public class TypeAhead {
	
	private static final Logger logger = Logger.getLogger(TypeAhead.class);
	
	private TernaryTree data;
	/**
	 * characters that we don't bother to concern
	 */
	private static final String NOT_UNDER_CONCERN = "[^0-9a-zA-Z]";		
	private static final String QUERY_PATTERN = "([a-zA-Z]){0,3}([0-9]){0,6}";	
	private static final String ONLY_NUMBER = "[0-9]{1,6}";	
	private static final String[] PREFIX = {"IDX","SZ","SH"};	
//	private static final Pattern QUERY_PATTERN = Pattern.compile("[a-zA-Z]{0,3}[0-9]{0,6}");
	
	@Autowired
	public TypeAhead(StockInfoDao stockInfoDao){
//		List<Stock> stocks = stockInfoDao.queryAllBasicInfo();
//		List<String> data = new ArrayList<String>();
//		if (stocks != null) {
//			for (Stock stock : stocks) {
//				data.add(stock.getName());
//			}
//			init(data);
//		}
	}
	
	public TypeAhead(List<String> names){
		init(names);
	}
	
	public void init(List<String> names){
		data = new TernaryTree();
		if(names != null){
			for (String string : names) {
				data.add(string);
			}
		}
	}
	
	/**
	 * deal with raw query string, replace non relevant character to space, split it to sub-query
	 * @param tofinds
	 * @return
	 */
	public List<String> query(String tofinds){
		ArrayList<String> results = new ArrayList<String>();		
		tofinds.replaceAll(NOT_UNDER_CONCERN, " ");	
		String[] subquerys = tofinds.split(" ");
		for (String subquery : subquerys) {
			if (subquery.matches(QUERY_PATTERN)) {
				results.addAll(querySingle(subquery));
			}
		}
		return results;
	}
	
	/**
	 * deal with single name query
	 * @param tofind
	 * @return
	 */
	private List<String> querySingle(String tofind){
		ArrayList<String> results = new ArrayList<String>();		
		if (tofind.matches(ONLY_NUMBER)) {
			results.addAll(autoAddPrefix(tofind)); 
		} else {
			List<String> finds = data.getCompletionsFor(tofind);
			if (finds != null) {
				results.addAll(finds);
			}
		}
		return results;
	}
	
	/**
	 * for numbers, should add prefix to look up in ternary tree
	 * @param numbers
	 * @return
	 */
	private List<String> autoAddPrefix(String numbers){
		ArrayList<String> results = new ArrayList<String>();		
		for (String prefix : PREFIX) {
			List<String> finds = data.getCompletionsFor(prefix + numbers);
			if (finds != null) {
				results.addAll(finds);
			}
		}	
		return results;
	}
}
