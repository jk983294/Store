package com.victor.utilities.math.opt;

import com.victor.utilities.math.utils.MathHelper;

import java.util.Random;

/**
 * base class for max optimizer, fitness bigger is better
 * you want to minimize, your fitness should multiply by -1.0
 */
public abstract class OptimizerBase <T extends Gene> {

    protected T current_params;
    protected T best_params;
    /**
     * parameter bound
     */
    protected double[] upbounds;
    protected double[] lowbounds;
    /*** gap between upbound and lowbound */
    protected double[] delta;
    /*** parameter count */
    protected int dimension;

    protected int iterateCount;

    protected final Random random = new Random();
    public final static double MUTATE_THRESHOLD = 0.2;

    public OptimizerBase(T init_params, double[] upbounds, double[] lowbounds) {
        this.current_params = init_params;
        this.upbounds = upbounds;
        this.lowbounds = lowbounds;
        dimension = upbounds.length;
        delta = new double[dimension];
        for (int i = 0; i < dimension; i++) {
            delta[i] = ( upbounds[i] - lowbounds[i] ) / 50;
        }
    }




    public void train() throws CloneNotSupportedException {
        initBeforeTrain();
        while ( ! isStopSatisfied() ){
            trainIteration();
        }
        processAfterTrain();
    }

    /**
     * mutate at index position
     */
    public void mutate(T gene, int index){
        if (random.nextDouble() < MUTATE_THRESHOLD){
            gene.setParam( index, MathHelper.randomRange(lowbounds[index], upbounds[index]) );
        } else {
            double newParam = delta[index] * ( random.nextDouble() - 0.5 ) + gene.getParam(index);
            // keep new param within range
            newParam = Math.max(newParam, lowbounds[index]);
            newParam = Math.min(newParam, upbounds[index]);
            gene.setParam( index,  newParam);
        }
    }

    public abstract void trainIteration() throws CloneNotSupportedException;

    public abstract void initBeforeTrain() throws CloneNotSupportedException;

    public abstract void processAfterTrain() throws CloneNotSupportedException;

    public abstract boolean isStopSatisfied();

    public T getBest_params() {
        return best_params;
    }

    public void setBest_params(T best_params) {
        this.best_params = best_params;
    }

    public double[] getUpbounds() {
        return upbounds;
    }

    public void setUpbounds(double[] upbounds) {
        this.upbounds = upbounds;
    }

    public double[] getLowbounds() {
        return lowbounds;
    }

    public void setLowbounds(double[] lowbounds) {
        this.lowbounds = lowbounds;
    }

    public int getIterateCount() {
        return iterateCount;
    }
}
