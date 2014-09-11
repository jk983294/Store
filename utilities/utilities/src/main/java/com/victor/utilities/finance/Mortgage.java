package com.victor.utilities.finance;

import java.util.ArrayList;
import java.util.List;



public class Mortgage {
	
	/**
	 * payment for each term
	 * @param pv - present value
	 * @param i - interest rate
	 * @param n - periods, years
	 * @return coupon payment for each period
	 */
	public static double paymentByPv(double pv, double i, double n) {
		double factor = 1 / ( 1 + i );
		return pv / ( factor * ( 1- Math.pow( factor, n)) / ( 1 - factor) );
	}
	
	/**
	 * payment for each term
	 * @param fv - future value
	 * @param i - interest rate
	 * @param n - periods, years
	 * @return coupon payment for each period
	 */
	public static double paymentByFv(double fv, double i, double n) {
		double factor = ( 1 + i );
		return fv / ( ( 1- Math.pow( factor, n)) / ( 1 - factor) );
	}
	
	/**
	 * 2 list, one is every period interest payment, another is every period
	 * principal payment
	 * @param pv
	 * @param i
	 * @param n
	 * @return
	 */
	public static List<double[]> mortgageReturnProcess(double pv, double i, int n) {
		double payment = paymentByPv(pv, i, n);
		double[] principalPay = new double[n];
		double[] interestPay = new double[n];
		for (int j = 0; j < n; j++) {
			interestPay[j] = pv * i;
			principalPay[j] = payment - interestPay[j];
			pv -= principalPay[j];
		}
		List<double[]> list = new ArrayList<>();
		list.add(interestPay);
		list.add(principalPay);
		return list;
	}
}
