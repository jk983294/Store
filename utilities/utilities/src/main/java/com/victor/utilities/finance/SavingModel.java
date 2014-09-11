package com.victor.utilities.finance;

public class SavingModel {

	/**
	 * what amount should deposit in order to get replacement% income after retirement 
	 * @param replacement - replacement rate compare with preretirement income
	 * @param i - real interest rate
	 * @param workyear - work year
	 * @param retireyear - retire year
	 * @return payment percentage with preretirement income 
	 */
	public static double replacementRateTarget(double replacement, double i, double workyear, double retireyear) {
		double pv = Annuity.pv(replacement, i, retireyear);
		double saveamount = Mortgage.paymentByFv(pv, i, workyear);
		return saveamount;
	}
	
	/**
	 * discounted income pv = discounted spend pv 
	 * @param income - income for work year
	 * @param i - real interest rate
	 * @param workyear - work year
	 * @param retireyear - retire year
	 * @return
	 */
	public static double sameConsumptionLevel(double income, double i, double workyear, double retireyear) {
		double pv = Annuity.pv(income, i, workyear);
		double spend = Mortgage.paymentByPv(pv, i, workyear + retireyear);
		double saveamount = income - spend;
		return saveamount;
	}
}
