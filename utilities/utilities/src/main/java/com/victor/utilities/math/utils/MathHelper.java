package com.victor.utilities.math.utils;

import org.apache.commons.math3.util.MathArrays;

import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Random;

/**
 * Math Utils
 */
public class MathHelper {

    private final static Random random = new Random();

    /**
     * generate uniform random variables in given bounds
     */
    public static double[] randomRange(double[] low, double[] up){
        int len = up.length;
        double[] rand = new double[len];
        for (int i = 0; i < len; i++) {
            rand[i] = randomRange(low[i], up[i]);
        }
        return rand;
    }

    public static double randomRange(double low, double up){
        return (up - low) * random.nextDouble() + low;
    }

    public static List<double[]> randomRangeList(int size, double[] low, double[] up){
        List<double[]> popList = new ArrayList<>();
        for (int i = 0; i < size; i++) {
            popList.add(randomRange(low, up));
        }
        return popList;
    }

    /**
     * swap elements in array
     */
    public static <T> void swap(int parentIndex, int childIndex, T[] array) {
        T parent = array[parentIndex];
        array[parentIndex] = array[childIndex];
        array[childIndex] = parent;
    }

    public static void swap(int parentIndex, int childIndex, double[] array) {
        double parent = array[parentIndex];
        array[parentIndex] = array[childIndex];
        array[childIndex] = parent;
    }

    /**
     * copy double array
     */
    public static double[] copy(double[] original){
        return MathArrays.copyOf(original);
    }

    public static double[] copy(double[] original, int len){
        return MathArrays.copyOf(original, len);
    }

    public static double[] copy(double[] original, int from, int len){
        return Arrays.copyOfRange(original, from, from + len);
    }

    /**
     * transform between list and array
     */
    public static <T> T[] list2array(List<T> original){
        T[] array = (T[]) Array.newInstance(original.get(0).getClass(), original.size());
        for (int i = 0; i < array.length; i++) {
            array[i] = original.get(i);
        }
        return array;
    }

    public static <T> List<T> array2list(T[] original){
        List<T> list = new ArrayList<>();
        for (int i = 0; i < original.length; i++) {
            list.add(original[i]);
        }
        return list;
    }


}
