package com.victor.finFriday.commonsmath;

import org.apache.commons.math3.linear.*;

public class Vector {
	
	public static void main(String[] args) {
		//ArrayRealVector and ArrayFieldVector ( generic ) implements RealVector interface
		vector();
	}
	
	private static void vector() {
		double[] vec1 = { 1d, 2d, 3d };
		double[] vec2 = { 4d, 5d, 6d };
		ArrayRealVector v1 = new ArrayRealVector(vec1);
		ArrayRealVector v2 = new ArrayRealVector(vec2);		
		System.out.println("size is " + v1.getDimension());			// dimension : size of vector		
		System.out.println("v1 + v2 = " + v1.add(v2));				// vector add	
		System.out.println("v1 - v2 = " + v1.subtract(v2));			// vector substract
		System.out.println("v1 * v2 = " + v1.ebeMultiply(v2));	// vector element by element multiply		
		System.out.println("v1 / v2 = " + v1.ebeDivide(v2));		// vector element by element divide
		System.out.println("v1[1] = " + v1.getEntry(1));				// get index at 1								
		System.out.println("v1 append v2 is " + v1.append(v2));// vector append
		// vector distance
		System.out.println("distance between v1 and v2 is "+ v1.getDistance(v2));
		System.out.println("L1 distance between v1 and v2 is "+ v1.getL1Distance(v2));
		
		System.out.println("norm of v1 is " + v1.getNorm());			// vector norm	
		System.out.println("dot product of v1 and v2 is " + v1.dotProduct(v2));		// vector dot product
		System.out.println("outer product of v1 and v2 is "+ v1.outerProduct(v2));	// vector outer product
		System.out.println("hogonal projection of v1 and v2 is "+ v1.projection(v2));		// vector orthogonal projection
		// vector map operations
//		System.out.println("Map the Math.abs(double) function to v1 is "+ v1.mapAbs());
//		v1.mapInvToSelf();
		System.out.println("Map the 1/x function to v1 itself is " + v1);
		// vector get sub vector
		System.out.println("sub vector of v1 is " + v1.getSubVector(0, 2));
	}
}
