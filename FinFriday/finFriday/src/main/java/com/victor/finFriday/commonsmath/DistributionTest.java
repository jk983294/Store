package com.victor.finFriday.commonsmath;

import org.apache.commons.math3.distribution.NormalDistribution;
import org.apache.commons.math3.distribution.PoissonDistribution;

public class DistributionTest {
	public static void main(String[] args) {
		test();
		System.out.println("-----------------------------------------------");
		poisson();
		System.out.println("-----------------------------------------------");
		normal();
	}
	/**
	 * test for example 《饮料装填量不足与超量的概率》
	 * 某饮料公司装瓶流程严谨，每罐饮料装填量符合平均600毫升，标准差3毫升的常态分配法则
	 * 。随机选取一罐，容量超过605毫升的概率？容量小于590毫升的概率 容量超过605毫升的概率 = p ( X > 605)= p ( ((X-μ)
	 * /σ) > ( (605 – 600) / 3) )= p ( Z > 5/3) = p( Z > 1.67) = 0.0475
	 * 容量小于590毫升的概率 = p (X < 590) = p ( ((X-μ) /σ) < ( (590 – 600) / 3) )= p ( Z
	 * < -10/3) = p( Z < -3.33) = 0.0004
	 */
	private static void test() {
		NormalDistribution normal = new NormalDistribution(600, 3);
		System.out.println("P(X<590) = " + normal.cumulativeProbability(590));
		System.out.println("P(X>605) = "
				+ (1 - normal.cumulativeProbability(605)));
	}

	private static void poisson() {
		PoissonDistribution dist = new PoissonDistribution(4.0);
		System.out.println("P(X<=2.0) = " + dist.cumulativeProbability(2));
		System.out.println("mean value is " + dist.getMean());
		System.out.println("P(X=1.0) = " + dist.probability(1));
		System.out.println("P(X=x)=0.8 where x = "
				+ dist.inverseCumulativeProbability(0.8));
	}

	private static void normal() {
		NormalDistribution normal = new NormalDistribution(0, 1);
		System.out.println("P(X<2.0) = "+ normal.cumulativeProbability(2.0));
		System.out.println("mean value is " + normal.getMean());
		System.out.println("standard deviation is "+ normal.getStandardDeviation());
		System.out.println("P(X=1) = " + normal.density(1.0));
		System.out.println("P(X<x)=0.8 where x = "+ normal.inverseCumulativeProbability(0.8));
	}
}
