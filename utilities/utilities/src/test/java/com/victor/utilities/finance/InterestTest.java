package com.victor.utilities.finance;

import static org.junit.Assert.assertEquals;

import org.junit.Test;

public class InterestTest {
	
	private final static double epsilon = 1e-4;

	@Test
	public void calcRealRateReturnTest(){
		assertEquals(0.02857142, InterestRate.calcRealRateReturn(0.08, 0.05), epsilon);
	}
	
	@Test
	public void futureValueTest(){
		assertEquals(1610.51, InterestRate.compoundInterestRate(1000, 0.1, 5), epsilon);
	}
	
	@Test
	public void effectiveAnnualRateTest(){
		assertEquals(0.0616778, InterestRate.effectiveAnnualRate( 0.06, 12), epsilon);
	}
	
	@Test
	public void effectiveAnnualRateInfinityTest(){
		assertEquals(0.0618365, InterestRate.effectiveAnnualRateInfinity( 0.06), epsilon);
	}
	
	@Test
	public void discountTest(){
		assertEquals(0.6806, InterestRate.discount(1, 0.08, 5), epsilon);
	}
	
	@Test
	public void yield2MaturityTest(){
		assertEquals(0.0592, InterestRate.yield2Maturity(75, 100, 5), epsilon);
	}
}
