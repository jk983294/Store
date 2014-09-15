package com.victor.utilities.math.opt;

/**
 * base calss for max optimizer, fitness bigger is better
 */
public abstract class OptimizerBase <T extends Gene> {

    protected T current_params;
    protected T best_params;
    protected double[] upbounds;
    protected double[] lowbounds;
    protected int dimenision;

    public OptimizerBase(T current_params, double[] upbounds, double[] lowbounds) {
        this.current_params = current_params;
        this.upbounds = upbounds;
        this.lowbounds = lowbounds;
        dimenision = upbounds.length;
    }




    public void train() throws CloneNotSupportedException {
        initBeforeTrain();
        while ( ! isStopSatisfied() ){
            trainIteration();
        }
        processAfterTrain();
    }

    public abstract void trainIteration();

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
}
