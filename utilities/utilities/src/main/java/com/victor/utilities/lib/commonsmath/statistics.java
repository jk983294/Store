package com.victor.utilities.lib.commonsmath;

import org.apache.commons.math3.stat.StatUtils;
import org.apache.commons.math3.stat.descriptive.DescriptiveStatistics;
import org.apache.commons.math3.stat.descriptive.StorelessUnivariateStatistic;
import org.apache.commons.math3.stat.descriptive.SummaryStatistics;
import org.apache.commons.math3.stat.descriptive.UnivariateStatistic;
import org.apache.commons.math3.stat.descriptive.moment.Mean;

public class statistics {
	
	/**
	 * min, max, mean, geometric mean, n, sum, sum of squares, standard deviation, variance, 
	 * percentiles, skewness, kurtosis, median
	 */
	public static void descriptive() {
		double[] values = new double[] { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
		UnivariateStatistic stat = new Mean();
		System.out.println("mean = " + stat.evaluate(values));
		System.out.println("mean = " + stat.evaluate(values,0,1));		// [ 0 , 1 ) useful for range
		
		StorelessUnivariateStatistic stat1 = new Mean();
		for (int i = 0; i < values.length; i++) {
			stat1.increment(values[i]);
		}
		System.out.println("mean after clear is NaN = "  + stat1.getResult());
		
		/**
		 * use DescriptiveStatistics
		 * min, max, mean, geometric mean, n, sum, sum of squares, standard deviation, 
		 * variance, percentiles, skewness, kurtosis, median
		 */
		DescriptiveStatistics descriptiveStatistics = new DescriptiveStatistics();
		for( int i = 0; i < values.length; i++) {
			descriptiveStatistics.addValue(values[i]);
		}
		System.out.println("std = "  + descriptiveStatistics.getStandardDeviation());
		System.out.println("median = "  + descriptiveStatistics.getPercentile(30));
		
		/**
		 * use  SummaryStatistics, read only once, no store overhead
		 * min, max, mean, geometric mean, n, sum, sum of squares, standard deviation, variance
		 */
		SummaryStatistics summaryStat = new SummaryStatistics();
		for( int i = 0; i < values.length; i++) {
			descriptiveStatistics.addValue(values[i]);
		}
		System.out.println(summaryStat.getMean());
		System.out.println(summaryStat.getStandardDeviation());
		
		/**
		 * use StatUtils 
		 */
		System.out.println(StatUtils.mean(values));
		System.out.println(StatUtils.variance(values));
		System.out.println(StatUtils.percentile(values,50));
		System.out.println(StatUtils.mean(values, 0 , 3));  // Compute the mean of the first three values in the array

		/**
		 * use DescriptiveStats  Maintain a "rolling mean" of the most recent 3 values from an input stream 
		 */
		DescriptiveStatistics descriptivestats = new DescriptiveStatistics();
		descriptivestats.setWindowSize(3);
		for( int i = 0; i < values.length; i++) {
			descriptivestats.addValue(values[i]);
			 System.out.println("rolling mean" + descriptivestats.getMean());
		}	
	}
	
	public static void frequency() {
		
	}
	
	public static void main(String[] args) {
		descriptive();
		frequency();
	}
}
