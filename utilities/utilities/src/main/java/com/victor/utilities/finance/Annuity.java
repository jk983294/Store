package com.victor.utilities.finance;

public class Annuity {

	/**
	 * growth perpetual
	 * @param dividend
	 * @param i interest rate
	 * @param g growth rate
	 * @return
	 */
	public static double yieldOnPreferredStock(double dividend, double i, double g) {
		return dividend / ( i - g );
	}
	
	public static double perpetualPV(double coupon, double i) {
		return coupon / i;
	}
	
	/**
	 * bond pv, each period yield coupon
	 * @param coupon
	 * @param i interest rate 
	 * @param n periods
	 * @return
	 */
	public static double pv(double coupon, double i, double n) {
		return coupon * ( 1 - Math.pow( 1 + i, -n )) / i;
	}
	
	public static double pv(double[] cash, double i) {
		double pv = 0;
		for (int j = 0; j < cash.length; j++) {
			pv += cash[j] / Math.pow( 1 + i, j); 
		}
		return pv;
	}
	
	public static double fv(double[] cash, double i) {
		double fv = 0;
		double m = cash.length;
		for (int j = 0; j < cash.length; j++) {
			fv += cash[j] * Math.pow( 1 + i, m - j); 
		}
		return fv;
	}
	
	
}
