package com.victor.utilities.lib.commonsmath;

import java.util.ArrayList;

import org.apache.commons.math3.analysis.UnivariateFunction;
import org.apache.commons.math3.optim.MaxEval;
import org.apache.commons.math3.optim.PointValuePair;
import org.apache.commons.math3.optim.linear.LinearConstraint;
import org.apache.commons.math3.optim.linear.LinearConstraintSet;
import org.apache.commons.math3.optim.linear.LinearObjectiveFunction;
import org.apache.commons.math3.optim.linear.NonNegativeConstraint;
import org.apache.commons.math3.optim.linear.PivotSelectionRule;
import org.apache.commons.math3.optim.linear.Relationship;
import org.apache.commons.math3.optim.linear.SimplexSolver;
import org.apache.commons.math3.optim.nonlinear.scalar.GoalType;
import org.apache.commons.math3.optim.univariate.BrentOptimizer;
import org.apache.commons.math3.optim.univariate.SearchInterval;
import org.apache.commons.math3.optim.univariate.UnivariateObjectiveFunction;
import org.apache.commons.math3.optim.univariate.UnivariateOptimizer;
import org.apache.commons.math3.optim.univariate.UnivariatePointValuePair;

import com.victor.utilities.visual.VisualAssist;

public class OptimizationDemo {
	
	public static void main(String[] args) {
		UnivariateOptimizerDemo();
		linearProgramming();
	}
	
	public static void UnivariateOptimizerDemo() {
		final UnivariateFunction f = new UnivariateFunction() {
            public double value(double x) {
                return x * x + 4 * x + 3;
            }
        };
	    UnivariateOptimizer optimizer = new BrentOptimizer(1e-10, 1e-8);
	    UnivariatePointValuePair result = optimizer.optimize(new MaxEval(10000),
                                             new UnivariateObjectiveFunction(f),
                                             GoalType.MINIMIZE,
                                             new SearchInterval(-1000, 1000));
	    System.out.println("UnivariateOptimizer for x*x + 4*x + 3 : " + result.getPoint() + " : " + result.getValue() );
	}
	
	public static void linearProgramming() {
        //      maximize 10 x1 - 57 x2 - 9 x3 - 24 x4
        //      subject to
        //          1/2 x1 - 11/2 x2 - 5/2 x3 + 9 x4  <= 0
        //          1/2 x1 -  3/2 x2 - 1/2 x3 +   x4  <= 0
        //              x1                  <= 1
        //      x1,x2,x3,x4 >= 0

        LinearObjectiveFunction f = new LinearObjectiveFunction(new double[] { 10, -57, -9, -24}, 0);
        
        ArrayList<LinearConstraint> constraints = new ArrayList<LinearConstraint>();

        constraints.add(new LinearConstraint(new double[] {0.5, -5.5, -2.5, 9}, Relationship.LEQ, 0));
        constraints.add(new LinearConstraint(new double[] {0.5, -1.5, -0.5, 1}, Relationship.LEQ, 0));
        constraints.add(new LinearConstraint(new double[] {  1,    0,    0, 0}, Relationship.LEQ, 1));
        
        SimplexSolver solver = new SimplexSolver();
        PointValuePair solution = solver.optimize(f, new LinearConstraintSet(constraints),
                                                  GoalType.MAXIMIZE,
                                                  new NonNegativeConstraint(true),
                                                  PivotSelectionRule.BLAND);
        VisualAssist.print("linearProgramming  points : ", solution.getPoint());
        System.out.println("linearProgramming  value : "  + solution.getValue() );      
	}
}
