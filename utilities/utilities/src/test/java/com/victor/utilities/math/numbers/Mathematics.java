package com.victor.utilities.math.numbers;

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import java.util.HashMap;
import java.util.Map;
import java.util.Random;

import org.junit.Test;


import com.victor.utilities.algorithm.dynamicprogramming.Knapsack;
import com.victor.utilities.math.Primes;

public class Mathematics {

    private static final int MIN = 1;
    private static final int MAX = 1000;

    private static final Random RANDOM = new Random();
    private static int nextRandomInt(int min, int max) {
        // nextInt is normally exclusive of the top value,
        // so add 1 to make it inclusive
        return RANDOM.nextInt((max - min) + 1) + min;
    }





    @Test
    public void knapsack() {
        int[] values = { 7, 4, 8, 6, 2, 5 };
        int[] weights = { 2, 3, 5, 4, 2, 3 };
        int capacity = 9;
        int[] result = Knapsack.zeroOneKnapsack(values, weights, capacity);
        int[] check = new int[]{5,3,0}; 
        for (int i=0; i<result.length; i++) {
            int r = result[i];
            int c = check[i];
            assertTrue("Knapsack problem. expected="+c+" got="+r, r==c);
        }
    }

    @Test
    public void getPrimeFactorization() {
        int number = 234;
        Map<Long,Long> factorization = Primes.getPrimeFactorization(number);
        Map<Long,Long> check = new HashMap<Long,Long>();
        {
            check.put(2l, 1L);
            check.put(3l, 2L);
            check.put(13L, 1L);
        }
        for (Long k : factorization.keySet()) {
            Long f = factorization.get(k);
            Long c = check.get(k);
            assertTrue("PrimeFactorization error. expected="+c+" got="+f, (c==f));
        }
    }

    @Test
    public void isPrime() {
        int number = 1234;
        boolean isPrime = Primes.isPrime(number);
        assertFalse("isPrime error. isPrime="+isPrime, isPrime);

        number = 7919;
        isPrime = Primes.isPrime(number);
        assertTrue("isPrime error. isPrime="+isPrime, isPrime);
    }

    @Test
    public void sieveOfEratosthenes() {
        int number = 4177;
        boolean isPrime = Primes.sieveOfEratosthenes(number);
        assertTrue("Sieve Of Eratosthenes error.", isPrime);

        number = 4178;
        isPrime = Primes.sieveOfEratosthenes(number);
        assertFalse("Sieve Of Eratosthenes error.", isPrime);

        number = 7919;
        isPrime = Primes.sieveOfEratosthenes(number);
        assertTrue("Sieve Of Eratosthenes error.", isPrime);

        number = 556;
        isPrime = Primes.sieveOfEratosthenes(number);
        assertFalse("Sieve Of Eratosthenes error.", isPrime);

        number = 6091;
        isPrime = Primes.sieveOfEratosthenes(number);
        assertTrue("Sieve Of Eratosthenes error.", isPrime);

        number = 6090;
        isPrime = Primes.sieveOfEratosthenes(number);
        assertFalse("Sieve Of Eratosthenes error.", isPrime);

        number = 6089;
        isPrime = Primes.sieveOfEratosthenes(number);
        assertTrue("Sieve Of Eratosthenes error.", isPrime);
    }
}
