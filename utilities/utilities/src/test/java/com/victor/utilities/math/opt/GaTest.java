package com.victor.utilities.math.opt;

/**
 * unit test for GA
 */
public class GaTest {

    public static void main(String[] args) throws CloneNotSupportedException {
        double[] initParams = new double[]{ 5, 5};
        double[] upbounds = new double[]{ 100, 100};
        double[] lowbounds = new double[]{ -100, -100};
        PolyGene initparam = new PolyGene(initParams,upbounds,lowbounds);
        GA<PolyGene> ga = new GA<>(initparam, upbounds, lowbounds);
        ga.train();
        System.out.println(ga.getBest_params().toString());
    }
}


