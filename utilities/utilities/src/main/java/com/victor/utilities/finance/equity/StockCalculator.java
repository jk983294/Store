package com.victor.utilities.finance.equity;

public class StockCalculator {
	
	public static double totalShareholderReturn(double endPrice, double startPrice, double dividend) {
		return ( endPrice - startPrice + dividend ) / startPrice;
	}
	
	/**
	 * return on equity
	 * @param netIncome firm net income this period
	 * @param equity initial book value of stock
	 * @return
	 */
	public static double roe(double netIncome, double equity) {
		return netIncome / equity;
	}
}
