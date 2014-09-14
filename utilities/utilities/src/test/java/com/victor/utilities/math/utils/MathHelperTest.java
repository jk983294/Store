package com.victor.utilities.math.utils;

import com.victor.utilities.visual.VisualAssist;
import org.junit.Test;

/**
 * unit test for MathHelper
 */
public class MathHelperTest {

    @Test
    public void testRandomRange(){
        System.out.println(MathHelper.randomRange(-10, 10));
        double[] ups = new double[]{ 1, 3, 5, 7};
        double[] downs = new double[]{ 6, 9, 12, 100};
        VisualAssist.print("random : ", MathHelper.randomRange(downs, ups));
    }


}
