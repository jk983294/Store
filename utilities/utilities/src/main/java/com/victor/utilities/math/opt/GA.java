package com.victor.utilities.math.opt;

import com.victor.utilities.algorithm.search.TopKElements;
import com.victor.utilities.math.utils.MathHelper;
import org.apache.commons.math3.util.FastMath;

import java.util.*;

/**
 * Genetic Algorithm
 */
public class GA <T extends Gene> extends OptimizerBase<T>  {

    /** the rate of crossover for the algorithm. */
    private final static double crossoverRate = 0.4;
    /** the rate of mutation for the algorithm. */
    private final static double mutationRate = 0.4;
    private final static double elitismRate = 0.4;
    private final static int TOURNAMENT_SELECT_NUM = 2;
    /** max iteration for generation */
    private final static int generationsMax = 150;
    /** the number of generations evolved to reach StoppingCondition in the last run. */
    private int generationsEvolved = 0;
    /** population size  */
    private final static int POPULATION_SIZE = 100;


    private List<T> population;


    public GA(T current_params, double[] upbounds, double[] lowbounds) {
        super(current_params, upbounds, lowbounds);
    }


    @Override
    public void trainIteration() throws CloneNotSupportedException {
        population = evolve();
//        System.out.println("current population after nextGenerations : "+population.toString());
        calculateFitness();
//        System.out.println("current population after calculateFitness : "+population.toString());
//        System.out.println("current best : " + getBest_params().toString());
        generationsEvolved++;
    }


    /**
     * calculate Fitness, record best gene
     */
    private void calculateFitness(){
        for( T gene : population){
            gene.objective();
        }
        best_params = MathHelper.max(population);
    }

    private List<T> evolve() throws CloneNotSupportedException {
        //pick Good Genes
        int topK = (int) FastMath.ceil( elitismRate * POPULATION_SIZE);
        T[] topKlist = TopKElements.getFirstK(population, topK);
        List<T> nextGeneration = MathHelper.array2list(topKlist);
//        System.out.println("up round population : "+population.toString());
//        System.out.println("top list : " + nextGeneration.toString());


        while (nextGeneration.size() < POPULATION_SIZE) {
            // select parent chromosomes

            T gene1 = (T) tournament().clone();
            T gene2 = (T) tournament().clone();

            // crossover?
            if (random.nextDouble() < crossoverRate) {
                // apply crossover policy to create two offspring
                crossover(gene1, gene2);
            }

            // mutation?
            if (random.nextDouble() < mutationRate) {
                // apply mutation policy to the chromosomes
                mutate(gene1, random.nextInt(dimension));
                mutate(gene2, random.nextInt(dimension));
            }

            // add the first chromosome to the population
            nextGeneration.add(gene1);
            // is there still a place for the second chromosome?
            if (nextGeneration.size() < POPULATION_SIZE) {
                // add the second chromosome to the population
                nextGeneration.add(gene2);
            }
        }
//        System.out.println("exit before nextGenerations : "+nextGeneration.toString());
        return nextGeneration;
    }



    /**
     * crossover two genes
     */
    private void crossover(T a, T b){
        // array representations of the parents
        double[] parent1Rep = a.getParam();
        double[] parent2Rep = b.getParam();

        // and of the children
        double[] child1Rep = new double[dimension];
        double[] child2Rep = new double[dimension];

        // select a crossover point at random (0 and length makes no sense)
        final int crossoverIndex = random.nextInt(dimension);

        // copy the first part
        for (int i = 0; i < crossoverIndex; i++) {
            child1Rep[i] = parent1Rep[i];
            child2Rep[i] = parent2Rep[i];
        }
        // and switch the second part
        for (int i = crossoverIndex; i < dimension; i++) {
            child1Rep[i] = parent2Rep[i];
            child2Rep[i] = parent1Rep[i];
        }
        a.setParam(child1Rep);
        b.setParam(child2Rep);
    }

    /**
     * tournament select policy
     */
    private T tournament(){
        List<T> choices = new ArrayList<T> ();
        // create a copy of the chromosome list
        Set<Integer> choose = new HashSet<>();
        while ( choose.size() < TOURNAMENT_SELECT_NUM) {
            choose.add( random.nextInt( POPULATION_SIZE ));
        }
        for(Integer i : choose){
            choices.add(population.get(i));
        }
        // the winner takes it all
        return MathHelper.max(choices);
    }

    @Override
    public void initBeforeTrain() throws CloneNotSupportedException {
        population = new ArrayList<>();
        List<double[]> list = MathHelper.randomRangeList(POPULATION_SIZE, lowbounds, upbounds);
        for(double[] initParam : list){
            T gene = (T) current_params.clone();
            gene.setParam(initParam);
            population.add(gene);
        }
        calculateFitness();
        generationsEvolved = 0;
    }

    @Override
    public void processAfterTrain() throws CloneNotSupportedException {

    }

    @Override
    public boolean isStopSatisfied() {
        return generationsEvolved > generationsMax;
    }
}
