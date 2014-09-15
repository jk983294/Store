package com.victor.utilities.math.opt;

import com.victor.utilities.algorithm.search.TopKElements;
import com.victor.utilities.math.utils.MathHelper;
import org.apache.commons.math3.util.FastMath;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;

/**
 * Genetic Algorithm
 */
public class GA <T extends Gene> extends OptimizerBase<T>  {

    /** the rate of crossover for the algorithm. */
    private final static double crossoverRate = 0.1;
    /** the rate of mutation for the algorithm. */
    private final static double mutationRate = 0.1;
    private final static double elitismRate = 0.2;
    private final static int TOURNAMENT_SELECT_NUM = 2;
    /** max iteration for generation */
    private final static int generationsMax = 5;
    /** the number of generations evolved to reach StoppingCondition in the last run. */
    private int generationsEvolved = 0;
    /** population size  */
    private final static int POPULATION_SIZE = 20;



    private final Random random = new Random();


    private List<T> population;


    public GA(T current_params, double[] upbounds, double[] lowbounds) {
        super(current_params, upbounds, lowbounds);
    }


    @Override
    public void trainIteration() {
        population = nextGenerations();
        calculateFitness();
        System.out.println(getBest_params().toString());
        generationsEvolved++;
    }


    private void calculateFitness(){
        for( T gene : population){
            gene.objective();
        }
        best_params = MathHelper.max(population);
    }

    private List<T> nextGenerations() {
        //pick Good Genes
        int topK = (int) FastMath.ceil( elitismRate * POPULATION_SIZE);
        T[] topKlist = TopKElements.getFirstK(population, topK);
        List<T> nextGeneration = MathHelper.array2list(topKlist);
        System.out.println(nextGeneration.toString());
 //       System.out.println(nextGeneration().toString());


        while (nextGeneration.size() < POPULATION_SIZE) {
            // select parent chromosomes
            T gene1 = tournament();
            T gene2 = tournament();

            // crossover?
            if (random.nextDouble() < crossoverRate) {
                // apply crossover policy to create two offspring
                crossover(gene1, gene2);
            }

            // mutation?
            if (random.nextDouble() < mutationRate) {
                // apply mutation policy to the chromosomes
                gene1.mutate(random.nextInt(dimenision));
                gene2.mutate(random.nextInt(dimenision));
            }

            // add the first chromosome to the population
            nextGeneration.add(gene1);
            // is there still a place for the second chromosome?
            if (nextGeneration.size() < POPULATION_SIZE) {
                // add the second chromosome to the population
                nextGeneration.add(gene2);
            }
        }
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
        double[] child1Rep = new double[dimenision];
        double[] child2Rep = new double[dimenision];

        // select a crossover point at random (0 and length makes no sense)
        final int crossoverIndex = random.nextInt(dimenision);

        // copy the first part
        for (int i = 0; i < crossoverIndex; i++) {
            child1Rep[i] = parent1Rep[i];
            child2Rep[i] = parent2Rep[i];
        }
        // and switch the second part
        for (int i = crossoverIndex; i < dimenision; i++) {
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
        List<T> chromosomes = new ArrayList<T> (population);
        for (int i=0; i< TOURNAMENT_SELECT_NUM; i++) {
            // select a random individual and add it to the tournament
            int rind = random.nextInt(chromosomes.size());
            choices.add(population.get(rind));
            // do not select it again
            chromosomes.remove(rind);
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
