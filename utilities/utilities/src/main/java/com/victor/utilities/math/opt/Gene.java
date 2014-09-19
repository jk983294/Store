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

    protected Gene(double[] param) {
        this.param = param;
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

    public double getParam(int index) {
        return param[index];
    }

    public void setParam(int index, double value ){
        param[index] = value;
    }

    public void setParam(double[] param) {
        this.param = param;
    }

    @Override
    public String toString() {
        return "Gene{" +
                "param=" + Arrays.toString(param) +
                ", fitness=" + fitness +
                '}';
    }
}
