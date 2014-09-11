package com.voctor.midas.util.algo;

import java.util.ArrayList;
import java.util.List;

import com.victor.midas.services.TypeAhead;

public class TypeAheadTest {

	public static void main(String[] args) {
		List<String> data = new ArrayList<String>();
		data.add("IDX999999");
		data.add("IDX999998");
		data.add("IDX999997");
		data.add("IDX995996");
		data.add("IDX996995");
		data.add("IDX997994");
		data.add("IDX999993");
		data.add("IDX199996");
		data.add("IDX299996");
		data.add("IDX399996");
		data.add("SZ000000");
		data.add("SZ100000");
		data.add("SZ200000");
		data.add("SZ030000");
		data.add("SZ040000");
		data.add("SZ005000");
		data.add("SZ000600");
		data.add("SZ007000");
		data.add("SZ008000");
		data.add("SH000500");
		TypeAhead tAhead = new TypeAhead(data);
		
		System.out.println(tAhead.query("id sz sh"));
		System.out.println(tAhead.query("idx199996 sz0050 00050 dsfdfh73 7h84"));
	}
}
