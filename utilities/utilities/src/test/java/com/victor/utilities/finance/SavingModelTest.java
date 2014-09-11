package com.victor.utilities.finance;

import static org.junit.Assert.assertEquals;

import org.junit.Test;

public class SavingModelTest {

	private final static double epsilon = 1e-4;
	
	@Test
	public void replacementRateTargetTest(){
		assertEquals(5645.85 / 30000 , SavingModel.replacementRateTarget(0.75, 0.03, 30, 15), epsilon);
	}
	
	
	@Test
	public void sameConsumptionLevelTest(){
		assertEquals(1.0 - 23982.22335 / 30000.0 , SavingModel.sameConsumptionLevel(1, 0.03, 30, 15), epsilon);
	}
}
