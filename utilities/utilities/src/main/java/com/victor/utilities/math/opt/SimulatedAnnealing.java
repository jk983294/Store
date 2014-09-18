package com.victor.utilities.math.opt;

/**
 * Simulated Annealing Algorithm for Solving FES problem
 * Optional parameters:
 * 	fes.sa.it = Initial Temperature (0, +inf), default to 0.93
 *  fes.sa.ft = Freezing Temperature (0, +inf), less than initial temperature, default to 2^-30
 *  fes.sa.phi = Temperature decreasing rate (0, 1), default to 0.95
 *  fes.sa.p = Perturb rate (0, +inf), high perturb rate result in more perturb, default 0.1
 */
public class SimulatedAnnealing <T extends Gene> extends OptimizerBase<T> {
    /*** Initial Temperature */
    private static final double INITIAL_TEMPERATURE = 100.0;

    /*** Freezing Temperature, less than initial temperature */
    private static final double FREEZING_TEMPERATURE = 1e-6;

    /*** Temperature decreasing rate, range (0, 1) */
    private static final double PHI  = 0.95;

    private static final int ITERATION_PER_TEMPERATURE = 100;

    private double temperature;

    public SimulatedAnnealing(T current_params, double[] upbounds, double[] lowbounds) {
        super(current_params, upbounds, lowbounds);
    }

    @Override
    public void trainIteration() throws CloneNotSupportedException {
        for(int i = 0; i < ITERATION_PER_TEMPERATURE; i++) {
            // perturb
            T perturbed = (T) current_params.clone();
            // mutate
            mutate(perturbed, random.nextInt(dimension));
            perturbed.objective();
            // delta OF1
            double change = perturbed.getFitness() - current_params.getFitness();
            // if better, accept
            if(change > 0) {
                current_params = perturbed;
            }   // otherwise accept with probability
            else if(random.nextDouble() < Math.pow(Math.E, change / temperature)){
                current_params = perturbed;
            }

            //save best so far
            if (current_params.getFitness() > best_params.getFitness()){
                best_params = (T) current_params.clone();
            }
        }
        // decrease temperature
        temperature *= PHI;
    }

    @Override
    public void initBeforeTrain() throws CloneNotSupportedException {
        current_params.objective();
        best_params = (T) current_params.clone();
        temperature = INITIAL_TEMPERATURE;
    }

    @Override
    public void processAfterTrain() throws CloneNotSupportedException {

    }

    @Override
    public boolean isStopSatisfied() {
        return temperature < FREEZING_TEMPERATURE;
    }
}
