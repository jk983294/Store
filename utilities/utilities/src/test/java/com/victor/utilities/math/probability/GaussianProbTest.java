package com.victor.utilities.math.probability;

public class GaussianProbTest {
	
	public static void main(String[] args) {
		double z     = 5.0;
        double mu    = 0.0;
        double sigma = 1;
        System.out.println(Gaussian.cdf(z, mu, sigma));
        double y = Gaussian.cdf(z);
        System.out.println(Gaussian.inverseCdf(y));
        System.out.println(GaussianProb.getInvCDF(y, true));
        
        
        System.out.println(Gaussian.norm2dCdf(-2.32634787, -1.73016947, 0.85440037));
	}
}
