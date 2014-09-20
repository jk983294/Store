package com.voctor.midas.worker;

import com.victor.midas.model.Stock;
import com.victor.midas.services.MktDataLoadService;

public class MktDataLodderTest {

	public static void main(String[] args) throws Exception {
		MktDataLoadService mLoader = new MktDataLoadService();
		Stock stock = mLoader.fromFile("D:/MktData/RawData/SZ/SZ000001.TXT");
		System.out.println(stock.toString());

        mLoader.fromDirectory("D:/MktData/RawDataSmallSetForTest","SZ");
	}

}
