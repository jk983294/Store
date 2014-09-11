package com.victor.utilities.finance;


/**
 * for one time shot, not annuity
 */
public class InterestRate {
	
	/**
	 * internal rate of return IRR, the interest makes the present value yield future value
	 * @param pv
	 * @param fv
	 * @param n
	 * @return
	 */
	public static double yield2Maturity(double pv, double fv, double n) {
		return Math.pow(fv / pv, 1/n) - 1;
	}
	
	/**
	 * discount
	 * @param fv future value
	 * @param i interest
	 * @param n period
	 * @return present value
	 */
	public static double discount(double fv, double i, double n) {
		return fv / Math.pow( 1 + i, n);
	}
	
	public static double discount(double i, double n) {
		return 1 / Math.pow( 1 + i, n);
	}
	
	/**
	 * pv
	 * @param fv
	 * @param apr annual percentage rate
	 * @param n years
	 * @param m number of periods in year
	 * @return pv
	 */
	public static double discount(double fv, double apr, double n, double m) {
		return fv / Math.pow( 1 + apr / m, n * m);
	}

	/**
	 * effective Annual Rate
	 * @param apr annual percentage rate
	 * @param m number of compound periods
	 * @return EFF effective Annual Rate
	 */
	public static double effectiveAnnualRate(double apr, double m) {
		return Math.pow( 1 + apr / m, m) - 1;
	}
	
	public static double effectiveAnnualRateInfinity(double apr) {
		return Math.exp(apr) - 1;
	}
	
	/**
	 * compound Interest Rate
	 * @param i interest rate
	 * @param n terms
	 * @return compound Interest Rate
	 */
	public static double compoundInterestRate(double i, double n) {
		return Math.pow(1 + i, n);
	}
	
	public static double compoundInterestRate(double principal, double i, double n) {
		return principal * compoundInterestRate(i, n);
	}
	
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
