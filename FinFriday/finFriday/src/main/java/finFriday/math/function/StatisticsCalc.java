package finFriday.math.function;

import org.apache.commons.math3.stat.descriptive.DescriptiveStatistics;

public class StatisticsCalc {

	/**
	 * first n-th result is using insufficient data for calculation
	 * for other result, it subtract first data out of range, then add new coming data,
	 * take average of that total 
	 * @param data
	 * @param interval
	 * @return
	 */
	public double[] mean(double[] data, int interval) {
		double[] result = new double[data.length];
		double total = 0;
		for (int i = 0; i < interval; ++i) {
			total += data[i];
			result[i]= total / (i + 1 ); 
		}
		for (int i = interval; i < data.length; ++i) {
			total -= data[ i - interval];
			total += data[i];
			result[i]= total / interval; 
		}
		return result;
	}
	
	public double[] mean1(double[] data, int interval) {
		double[] result = new double[data.length];
		DescriptiveStatistics descriptivestats = new DescriptiveStatistics();
		descriptivestats.setWindowSize(interval);
		for( int i = 0; i < data.length; i++) {
			descriptivestats.addValue(data[i]);
			result[i] = descriptivestats.getMean();
		}	
		return result;
	}
}
