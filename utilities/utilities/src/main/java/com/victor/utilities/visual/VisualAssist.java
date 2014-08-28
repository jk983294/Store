package com.victor.utilities.visual;

public class VisualAssist {
	
	public static void print(double[] a) {
		for (double d : a) {
			System.out.print(d + " ");
		}
		System.out.println();
	}
	
	public static void print(String description, double[] a) {
		System.out.println(description);
		print(a);
	}
}
