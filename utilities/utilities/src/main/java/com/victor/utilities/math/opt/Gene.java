package com.victor.utilities.math.opt;

import com.victor.utilities.math.utils.MathHelper;

import java.util.Arrays;
import java.util.Random;

/**
 * gene, used to represent something could be optimized
 * this thing have param vector and fitness value for that param
 */
public abstract class Gene <E extends Gene> implements Comparable<E> , Cloneable  {

    protected double[] param;
    protected double fitness;
    protected double[] upbounds;
    protected double[] lowbounds;

    private final static Random random = new Random();
    private final static double MUTATE_THRESHOLD = 0.2;

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



    @Override
    protected Object clone() throws CloneNotSupportedException {
        // upbounds and lowbounds are shallow copied shared by all genes, but param should be deep copy
        Gene cloned = (Gene) super.clone();
        cloned.setParam(MathHelper.copy(param));
        return cloned;
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

    public void setParam(int index, double value ){
        param[index] = value;
    }

    public void setParamDelta(int index, double delta ){
        param[index] += delta;
        // keep new param within range
        param[index] = Math.max(param[index], lowbounds[index]);
        param[index] = Math.min(param[index], upbounds[index]);
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
