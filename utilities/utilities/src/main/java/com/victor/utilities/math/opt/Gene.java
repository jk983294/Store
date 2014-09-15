package com.victor.utilities.math.opt;

import com.victor.utilities.math.utils.MathHelper;

import java.util.Arrays;

/**
 * gene, used to represent something could be optimized
 * this thing have param vector and fitness value for that param
 */
public abstract class Gene <E extends Gene> implements Comparable<E> , Cloneable  {

    protected double[] param;
    protected double fitness;
    protected double[] upbounds;
    protected double[] lowbounds;

    protected Gene(double[] param, double[] upbounds, double[] lowbounds) {
        this.param = param;
        this.upbounds = upbounds;
        this.lowbounds = lowbounds;
    }

    /**
     * derived class should give meaning of fitness, how the param affect the fitness
     * it should record fitness for next comparison
     */
    public abstract void objective();

    /**
     * mutate at index position
     */
    public void mutate(int index){
        param[index] = MathHelper.randomRange(lowbounds[index], upbounds[index]);
    }

    @Override
    protected Object clone() throws CloneNotSupportedException {
        return super.clone();
    }

    @Override
    public int compareTo(Gene another) {
        return Double.compare(getFitness(), another.getFitness());
    }

    public double getFitness() {
        return fitness;
    }

    public double[] getParam() {
        return param;
    }

    public void setParam(double[] param) {
        this.param = param;
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

    @Override
    public String toString() {
        return "Gene{" +
                "param=" + Arrays.toString(param) +
                ", fitness=" + fitness +
                '}';
    }
}
