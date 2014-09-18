package com.victor.utilities.math.opt;

/**
 * unit test for Simulated Annealing
 */
public class SimulatedAnnealingTest {

    public static void main(String[] args) throws CloneNotSupportedException {
        double[] initParams = new double[]{ 5, 5};
        double[] upbounds = new double[]{ 100, 100};
        double[] lowbounds = new double[]{ -100, -100};
        PolyGene initparam = new PolyGene(initParams,upbounds,lowbounds);
        SimulatedAnnealing<PolyGene> sa = new SimulatedAnnealing<>(initparam, upbounds, lowbounds);
        sa.train();
        System.out.println(sa.getBest_params().toString());
    }

}
