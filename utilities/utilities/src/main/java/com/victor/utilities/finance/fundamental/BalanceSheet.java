package com.victor.utilities.finance.fundamental;

public class BalanceSheet {

	public static double totalCurrentAsset(double securities, double receivables, double inventories) {
		return securities + receivables + inventories;
	}
	
	/**
	 * totalFixedAsset = net property, plant and equipment(PP&E)
	 * @param PPandE (PP&E)
	 * @param depreciation   less accumulated depreciation
	 * @return
	 */
	public static double totalFixedAsset(double PPandE, double depreciation) {
		return PPandE - depreciation;
	}
	
	public static double totalAsset(double totalCurrentAsset, double totalFixedAsset) {
		return totalCurrentAsset + totalFixedAsset;
	}
	
	public static double totoalCurrentLiabilities(double accountsPayable, double shortTermDebt) {
		return accountsPayable + shortTermDebt;
	}
	
	public static double totalLiabilities(double longTermDebt, double totoalCurrentLiabilities) {
		return longTermDebt + totoalCurrentLiabilities;
	}
	
	public static double equity(double paidInCapital, double retainedEarnings) {
		return paidInCapital + retainedEarnings;
	}
}
