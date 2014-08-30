package com.victor.utilities.finance;

import static org.junit.Assert.assertEquals;

import org.junit.Test;

public class InterestTest {
	
	private final static double epsilon = 1e-6;

	@Test
	public void calcRealRateReturnTest(){
		assertEquals(0.02857142, InterestRate.calcRealRateReturn(0.08, 0.05), epsilon);
	}
}
