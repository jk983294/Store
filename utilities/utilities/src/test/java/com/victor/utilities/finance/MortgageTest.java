package com.victor.utilities.finance;

import static org.junit.Assert.assertEquals;

import java.util.List;

import org.junit.Test;

import com.victor.utilities.visual.VisualAssist;

public class MortgageTest {
	private final static double epsilon = 1e-4;
	
	@Test
	public void paymentTest(){
		assertEquals(0.3950548, Mortgage.paymentByPv(1, 0.09, 3), epsilon);
	}
	
	@Test
	public void processTest(){
		List<double[]> process = Mortgage.mortgageReturnProcess(1, 0.09, 3);
		VisualAssist.print("interset pay : ", process.get(0));
		VisualAssist.print("principal pay : ", process.get(1));
	}
}
