package com.voctor.midas.worker;

import com.victor.midas.model.Stock;
import com.victor.midas.worker.MktDataLoader;

public class MktDataLodderTest {

	public static void main(String[] args) throws Exception {
		MktDataLoader mLoader = new MktDataLoader();
		Stock stock = mLoader.fromFile("D:/MktData/RawData/SZ/SZ000001.TXT");
		System.out.println(stock.toString());
		
		mLoader.fromDirectory("D:/MktData/RawDataSmallSetForTest");
	}

}
