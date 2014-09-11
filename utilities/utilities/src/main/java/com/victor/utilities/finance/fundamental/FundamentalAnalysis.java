package com.victor.utilities.finance.fundamental;

public class FundamentalAnalysis {

	/**
	 * sustainableGrowthRate = earningsRetentionRate * ROE
	 * @param earningsRetentionRate
	 * @param ROE return on equity
	 * @return
	 */
	public static double sustainableGrowthRate(double earningsRetentionRate, double ROE) {
		return earningsRetentionRate * ROE;
	}
	
	/**
	 * earningsRetentionRate = 1 - dividendPayoutRate - shareRepurchaseRate
	 * @param dividendPayoutRate
	 * @return
	 */
	public static double earningsRetentionRate(double dividendPayoutRate, double shareRepurchaseRate) {
		return 1 - dividendPayoutRate - shareRepurchaseRate;
	}
}
