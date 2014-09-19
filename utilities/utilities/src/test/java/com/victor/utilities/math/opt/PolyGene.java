package com.victor.utilities.math.opt;

/**
 * unit test for opt package, served as a polynomial function to optimize
 */
public class PolyGene extends Gene {

    private double[] testdata;

    protected PolyGene(double[] param) {
        super(param);
        testdata = new double[]{1.0, 2.0};
    }

    @Override
    public void objective() {
        double x = param[0];
        double y = param[1];
        double offset = testdata[0] + testdata[1];
        fitness = x * x + y * y - 3 + offset;

        fitness = -fitness;     // max
    }



}
