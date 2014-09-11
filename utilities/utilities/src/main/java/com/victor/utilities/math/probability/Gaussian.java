package com.victor.utilities.math.probability;

/*************************************************************************
 *  Compilation:  javac Gaussian.java
 *  Execution:    java Gaussian x mu sigma
 *
 *  Function to compute the Gaussian pdf (probability density function)
 *  and the Gaussian cdf (cumulative density function)
 *
 *  % java Gaussian 820 1019 209
 *  0.17050966869132111
 *
 *  % java Gaussian 1500 1019 209
 *  0.9893164837383883
 *
 *  % java Gaussian 1500 1025 231
 *  0.9801220907365489
 *
 *  The approximation is accurate to absolute error less than 8 * 10^(-16).
 *  Reference: Evaluating the Normal Distribution by George Marsaglia.
 *  http://www.jstatsoft.org/v11/a04/paper
 *
 *************************************************************************/

public class Gaussian {

    // return phi(x) = standard Gaussian pdf
    public static double density(double x) {
        return Math.exp(-x*x / 2) / Math.sqrt(2 * Math.PI);
    }

    // return phi(x, mu, signma) = Gaussian pdf with mean mu and stddev sigma
    public static double density(double x, double mu, double sigma) {
        return density((x - mu) / sigma) / sigma;
    }

    // return Phi(z) = standard Gaussian cdf using Taylor approximation
    public static double cdf(double z) {
        if (z < -8.0) return 0.0;
        if (z >  8.0) return 1.0;
        double sum = 0.0, term = z;
        for (int i = 3; sum + term != sum; i += 2) {
            sum  = sum + term;
            term = term * z * z / i;
        }
        return 0.5 + sum * density(z);
    }

    // return Phi(z, mu, sigma) = Gaussian cdf with mean mu and stddev sigma
    public static double cdf(double z, double mu, double sigma) {
        return cdf((z - mu) / sigma);
    } 

    // Compute z such that Phi(z) = y via bisection search
    public static double inverseCdf(double y) {
        return inverseCdf(y, .00000001, -8, 8);
    } 

    // bisection search
    private static double inverseCdf(double y, double delta, double lo, double hi) {
        double mid = lo + (hi - lo) / 2;
        if (hi - lo < delta) return mid;
        if (cdf(mid) > y) return inverseCdf(y, delta, lo, mid);
        else              return inverseCdf(y, delta, mid, hi);
    }
    
    /**
     * calculate two dimension gaussian distribution cdf
     * @param a random variable 
     * @param b random variable 
     * @param r sqrt of correlation
     * @return
     */
    public static double norm2dCdf(double a, double b, double r) {
		final double x[] = new double[] { 0, 0.04691008, 0.23076534, 0.5, 0.76923466, 0.95308992};
		final double w[] = new double[] { 0, 0.018854042, 0.038088059, 0.0452707394, 0.038088059, 0.018854042};
		double h1, h2, LH = 0, h12, h3, h5, h6, h7, r1, r2, r3, rr, AA, ab, result;
		h1 = a;
		h2 = b;
		h12 = ( h1 * h1 + h2 * h2 ) / 2;
		if ( Math.abs(r) >= 0.7 ) {
			r2 = 1 - r * r;
			r3 = Math.sqrt(r2);
			if (r < 0) {
				h2 = - h2;
			}
			h3 = h1 * h2;
			h7 = Math.exp( - h3 / 2);
			if (Math.abs(r) < 1) {
				h6 = Math.abs( h1 - h2);
				h5 = h6 * h6 / 2;
				h6 = h6 / r3;
				AA = 0.5 - h3 /8;
				ab = 3 - 2 * AA * h5;
				LH = 0.13298076 * h6 * ab * ( 1 - cdf(h6)) - Math.exp( - h5 / r2) * ( ab + AA * r2 ) * 0.053051647;
				for (int i = 0; i < x.length; i++) {
					r1 = r3 * x[i];
					rr = r1 * r1;
					r2 = Math.sqrt( 1 - rr );
					LH -= - w[i]*Math.exp(-h5/rr) * (Math.exp(-h3/(1+r2) / r2 / h7 - 1 - AA * rr));	
				}
			}
			result = LH * r3 * h7 + cdf(Math.min(h1, h2));
			if (r < 0) {
				result = cdf(h1) - result;
			}
		} else {
			h3 = h1 * h12;
			if (Math.abs(r) > 1e-6) {
				for (int i = 0; i < x.length; i++) {
					r1 = r * x[i];
					r2 = 1 - r1 * r1;
					LH += w[i]* Math.exp((r1*h3 - h12) / r2) / Math.sqrt(r2); 
				}
			}
			result = cdf(h1) * cdf(h2) + r * LH;
		}
		return result;
	}
    

}
