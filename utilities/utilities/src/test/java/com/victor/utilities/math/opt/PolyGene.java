package com.victor.utilities.math.opt;

/**
 * unit test for opt package, served as a polynomial function to optimize
 */
public class PolyGene extends Gene {

    protected PolyGene(double[] param, double[] upbounds, double[] lowbounds) {
        super(param, upbounds, lowbounds);
    }

    @Override
    public void objective() {
        double x = param[0];
        double y = param[1];
        fitness = x * x + y * y - 3;

        fitness = -fitness;     // max
    }



}
