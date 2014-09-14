package com.victor.utilities.algorithm.search;

import com.victor.utilities.visual.VisualAssist;
import org.junit.Test;

/**
 * unit test for TopKElements
 */
public class TopKElementsTest {


    @Test
    public void topKTest(){
        Double[] unsorted = new Double[]{ 1.0, 3.0, 5.0, 7.0, 9.0, 2.0, 4.0, 6.0, 8.0, 10.0, 12.0, 14.0, 16.0, 18.0, 11.0, 13.0, 13.0, 15.0,17.0};
        VisualAssist.print("sorted : " , TopKElements.getFirstK(unsorted, 5));
        VisualAssist.print("sorted : " , TopKElements.getFirstK(unsorted, 15));

        double[] unsorted1 = new double[]{ 1.0, 3.0, 5.0, 7.0, 9.0, 2.0, 4.0, 6.0, 8.0, 10.0, 12.0, 14.0, 16.0, 18.0, 11.0, 13.0, 13.0, 15.0,17.0};
        VisualAssist.print("sorted : " , TopKElements.getFirstK(unsorted1, 5));
        VisualAssist.print("sorted : " , TopKElements.getFirstK(unsorted1, 15));
    }
}
