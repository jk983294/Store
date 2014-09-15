package com.victor.utilities.lib.commonsmath;

import org.apache.commons.lang3.ArrayUtils;
import org.apache.commons.math3.exception.util.LocalizedFormats;
import org.apache.commons.math3.genetics.*;

import java.util.ArrayList;
import java.util.List;

/**
 * demo of generic algorithm framework of commonsmath
 */
public class GAdemo {

    // parameters for the GA
    private static final int DIMENSION = 2;
    private static final int POPULATION_SIZE = 80;
    private static final int NUM_GENERATIONS = 200;
    private static final double ELITISM_RATE = 0.2;
    private static final double CROSSOVER_RATE = 1;
    private static final double MUTATION_RATE = 0.08;
    private static final int TOURNAMENT_ARITY = 2;


    public static void main(String[] args) {
        // initialize a new genetic algorithm
        GeneticAlgorithm ga = new GeneticAlgorithm(
                new OnePointCrossover<Double>(),
                CROSSOVER_RATE,
                new RandomKeyMutation(),
                MUTATION_RATE,
                new TournamentSelection(TOURNAMENT_ARITY)
        );

        // initial population
        Population initial = randomInitialPopulation();

        // stopping condition
        StoppingCondition stopCond = new FixedGenerationCount(NUM_GENERATIONS);

        // best initial chromosome
        Chromosome bestInitial = initial.getFittestChromosome();

        System.out.println(bestInitial);

        // run the algorithm
        Population finalPopulation = ga.evolve(initial, stopCond);

        // best chromosome from the final population
        Chromosome bestFinal = finalPopulation.getFittestChromosome();

        System.out.println(bestFinal);
    }

    /**
     * Initializes a random population
     */
    private static ElitisticListPopulation randomInitialPopulation() {
        List<Chromosome> popList = new ArrayList<Chromosome>();
        for (int i=0; i<POPULATION_SIZE; i++) {
            Chromosome randChrom = new chromosomeDouble(RandomKey.randomPermutation(DIMENSION));
            popList.add(randChrom);
        }
        return new ElitisticListPopulation(popList, popList.size(), ELITISM_RATE);
    }


    /**
     * Chromosomes Double
     */
    private static class chromosomeDouble extends AbstractListChromosome<Double> {

        private double x;
        private double y;

        public chromosomeDouble(Double[] representation) throws InvalidRepresentationException {
            super(representation);
        }

        private chromosomeDouble(List<Double> representation) throws InvalidRepresentationException {
            super(representation);
        }

        @Override
        protected void checkValidity(List<Double> chromosomeRepresentation) throws InvalidRepresentationException {
            for (double val : chromosomeRepresentation) {
                if (val < 0 || val > 1) {
                    throw new InvalidRepresentationException(LocalizedFormats.OUT_OF_RANGE_SIMPLE,
                            val, 0, 1);
                }
            }
        }

        @Override
        public AbstractListChromosome<Double> newFixedLengthChromosome(List<Double> chromosomeRepresentation) {
            return new chromosomeDouble(chromosomeRepresentation);
        }

        @Override
        public double fitness() {
            List<Double> list = getRepresentation();
            Double[] d = list.toArray(new Double[list.size()]);
            double[] values = ArrayUtils.toPrimitive(d);
            x = values[0];
            y = values[1];

            return x * x + y * y - 1;
        }
    }
}
