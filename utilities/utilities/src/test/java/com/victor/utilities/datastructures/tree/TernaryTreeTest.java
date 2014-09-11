package com.victor.utilities.datastructures.tree;



public class TernaryTreeTest {
	public static void main(String[] args) {
		TernaryTree tt = new TernaryTree();
		tt.add("IDX999999");
		tt.add("IDX999998");
		tt.add("IDX999997");
		tt.add("IDX995996");
		tt.add("IDX996995");
		tt.add("IDX997994");
		tt.add("IDX999993");
		tt.add("IDX199996");
		tt.add("IDX299996");
		tt.add("IDX399996");
		tt.add("SZ000000");
		tt.add("SZ100000");
		tt.add("SZ200000");
		tt.add("SZ030000");
		tt.add("SZ040000");
		tt.add("SZ005000");
		tt.add("SZ000600");
		tt.add("SZ007000");
		tt.add("SZ008000");
		tt.BFS();
		
		System.out.println(tt.contains("SZ100000"));
		System.out.println(tt.contains("IDX399"));
		
		System.out.println(tt.getCompletionsFor("id"));
		System.out.println(tt.getCompletionsFor("idx199996"));
	}
}
