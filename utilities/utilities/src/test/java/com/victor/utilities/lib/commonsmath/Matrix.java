package com.victor.utilities.lib.commonsmath;

import org.apache.commons.math3.linear.*;

public class Matrix {

	public static void main(String[] args) {
		matrix();
		System.out.println("\n\n\n");
		matrixDecomposite();
	}

	private static void matrixDecomposite() {
		double[][] testData = { { 1.0, 2.0, 3.0 }, { 2.0, 5.0, 3.0 },
				{ 1.0, 0.0, 8.0 } };

		RealMatrix matrix = MatrixUtils.createRealMatrix(testData);
		// LUP decomposition
		LUDecomposition LU = new LUDecomposition(matrix);
		RealMatrix l = LU.getL();
		RealMatrix u = LU.getU();
		RealMatrix p = LU.getP();
		System.out.println("L is : " + l);
		System.out.println("U is : " + u);
		System.out.println("P is : " + p);
		System.out.println("PA is " + (p.multiply(matrix)));
		System.out.println("LU is " + (l.multiply(u)));
		System.out.println("PA = LU is "
				+ p.multiply(matrix).equals(l.multiply(u)));
		// matrix singular
		System.out.println("LU is not singular : "
				+ LU.getSolver().isNonSingular());
		// matrix determinant
		System.out.println("matrix determinant is : " + LU.getDeterminant());
		// matrix solver
		RealMatrix b = MatrixUtils.createRealMatrix(new double[][] { { 1, 0 },
				{ 2, -5 }, { 3, 1 } });
		DecompositionSolver solver = LU.getSolver();
		System.out.println("solve Ax = b (when b is matrix) is x = " + solver.solve(b));
		System.out.println("solve Ax = b (when b is vector) is x = "
				+ new ArrayRealVector(solver.solve(new ArrayRealVector(b.getColumn(0)))));
		// matrix inverse
		System.out.println("matrix inverse is " + solver.getInverse());
	}

	public static void matrix() {
		double[][] data1 = { { 1d, 2d, 3d }, { 2d, 5d, 3d }, { 1d, 0d, 8d } };
		double[][] t_data = { { -40d, 16d, 9d }, { 13d, -5d, -3d },
				{ 5d, -2d, -1d } };
		Array2DRowRealMatrix matrix1 = new Array2DRowRealMatrix(data1);
		Array2DRowRealMatrix t_mat = new Array2DRowRealMatrix(t_data);
		// is square
		System.out.println("it is square matrix! : " + matrix1.isSquare());
		// dimension of row and column
		System.out.println("row dimension is " + matrix1.getRowDimension());
		System.out.println("column dimension is "
				+ matrix1.getColumnDimension());
		// matrix add
		System.out.println("mat1 + mat1 = " + matrix1.add(matrix1));
		System.out.println("mat1 + 5 = " + matrix1.scalarAdd(5.0));
		// matrix sub
		System.out.println("mat1 - mat1 = " + matrix1.subtract(matrix1));
		// matrix norm
		System.out.println("the maximum absolute row sum norm is "
				+ matrix1.getNorm());
		// matrix multiply
		System.out.println("mat1 * t_mat = " + matrix1.multiply(t_mat));
		System.out.println("mat1 * 5.0 = " + matrix1.scalarMultiply(5));
		System.out.println("t_mat * mat1 = " + matrix1.preMultiply(t_mat));
		// matrix trace
		System.out.println("the trace is " + matrix1.getTrace());
		// matrix transpose
		System.out.println("the transpose of mat1 is " + matrix1.transpose());
		// matrix to vector
		System.out
				.println("the first row vector is " + matrix1.getRowVector(0));
		// matrix get sub matrix of selected rows and columns
		System.out.println("sub matrix of mat1 is "
				+ matrix1.getSubMatrix(new int[] { 0, 2 }, new int[] { 1, 2 }));
	}
}
