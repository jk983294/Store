package com.victor.utilities.finance;

import static org.junit.Assert.assertEquals;

import org.junit.Test;

public class AnnuityTest {
	private final static double epsilon = 1e-4;
	
	@Test
	public void fvTest(){
		double[] cashflow = new double[]{ 1000, 1000 };
		assertEquals(2310, Annuity.fv(cashflow, 0.1), epsilon);
	}
	
	@Test
	public void pvTest(){
		double[] cashflow = new double[]{ 0, 1000, 2000 };
		assertEquals(2561.98347107438, Annuity.pv(cashflow, 0.1), epsilon);
	}
	
	@Test
	public void pv1Test(){
		System.out.println(Annuity.pv(100, 0.1 , 3));
		assertEquals(248.68519909842243, Annuity.pv(100, 0.1 , 3), epsilon);
	}
	
	@Test
	public void yieldOnPreferredStockTest(){
		assertEquals(20000, Annuity.yieldOnPreferredStock(1000, 0.09 , 0.04), epsilon);
	}
}
