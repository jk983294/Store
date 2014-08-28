package com.victor.utilities.finance;

public class Interest {

	/**
	 * calculate Real Rate of Return, could derived from TIPS (treasury inflation protected securities)
	 * @param nominalInterestRate
	 * @param inflationRate usual use CPI 
	 * @return
	 */
	public static double calcRealRateReturn(double nominalInterestRate, double inflationRate) {
		return (1 + nominalInterestRate) / (1 + inflationRate) - 1;
	}
	
	/**
	 * riskPremium means higher risk, high rate of return 
	 * @param riskyRateReturn
	 * @param risklessRateReturn
	 * @return
	 */
	public static double calcRiskPremium(double riskyRateReturn, double risklessRateReturn) {
		return riskyRateReturn - risklessRateReturn;
	}
}
