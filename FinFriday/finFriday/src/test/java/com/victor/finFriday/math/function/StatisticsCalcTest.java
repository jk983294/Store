package com.victor.finFriday.math.function;


import finFriday.math.function.StatisticsCalc;

public class StatisticsCalcTest {

	
	public static void main(String[] args) {
		StatisticsCalc statisticsCalc = new StatisticsCalc();
		double[] data = {1,2,3,4,5,6,7,8,9};
		double[] result = statisticsCalc.mean1(data, 3);
		for (double d : result) {
			System.out.println(d);
		}
		System.out.println(result.toString());
	}
}
