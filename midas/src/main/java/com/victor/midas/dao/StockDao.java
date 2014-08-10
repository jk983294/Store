package com.victor.midas.dao;

import java.util.List;

import org.springframework.beans.factory.annotation.*;
import org.springframework.data.domain.Sort;
import org.springframework.data.domain.Sort.Direction;
import org.springframework.data.mongodb.core.*;
import org.springframework.data.mongodb.core.index.Index;
import org.springframework.data.mongodb.core.query.*;
import org.apache.log4j.Logger;

import com.victor.midas.calculator.IndexCalculator;
import com.victor.midas.model.*;

public class StockDao {
	private final String STOCKS_COLLECTION = "stocks"; 
	
	private final Logger logger = Logger.getLogger(StockDao.class);
	
	private final IndexCalculator calculator = new IndexCalculator();
	
	
	@Autowired  
    MongoTemplate mongoTemplate; 
	
	public void createCollection(){
		if (!mongoTemplate.collectionExists(STOCKS_COLLECTION)) {
			logger.info("mongoTemplate create collection");
            mongoTemplate.createCollection(STOCKS_COLLECTION);
            IndexOperations io = mongoTemplate.indexOps(STOCKS_COLLECTION);
            Index index =new Index();
            index.on("latest.change", Direction.DESC);
            io.ensureIndex(index);
        }
	}
	
	public void dropCollection(){
		if (mongoTemplate.collectionExists(STOCKS_COLLECTION)) {
            mongoTemplate.dropCollection(STOCKS_COLLECTION);
        }
	}
	
	public void saveStock(Stock stock){
		if (stock != null && stock.getRecords()!= null && stock.getRecords().size() > 0) {
			calculator.calculate(stock);
			stock.setLatest(stock.getRecords().get(stock.getRecords().size()-1));
		}
		mongoTemplate.save(stock, STOCKS_COLLECTION);
	}
	
	public Stock queryByName(String name){  
		return mongoTemplate.findOne(new Query(Criteria.where("_id").is(name)), Stock.class, STOCKS_COLLECTION);     
    } 
	
	/**
     * ·ÖÒ³²Ù×÷skip(first).limit(end)
     */
    public List<Stock> getStockByPaging(int first, int end) {
    	Query query =new Query().skip(first).limit(end);
		query.fields().include("name").include("desp").include("latest");
        return mongoTemplate.find(query, Stock.class, STOCKS_COLLECTION);
    }
	
	public List<Stock> queryAllBasicInfo(){  
		Query query =new Query();
		query.fields().include("name").include("desp").include("latest");
		return mongoTemplate.find(query,Stock.class, STOCKS_COLLECTION);     
    } 
	
	public List<Stock> queryAllName4AutoCompletion(){  
		Query query =new Query();
		query.fields().include("name");
		return mongoTemplate.find(query,Stock.class, STOCKS_COLLECTION);     
    } 
	
	public int getStockCount() {
        return (int) mongoTemplate.count( new Query(), STOCKS_COLLECTION);
    }
}
