package com.victor.midas.dao;

import java.util.List;

import org.springframework.beans.factory.annotation.*;
import org.springframework.data.domain.Sort.Direction;
import org.springframework.data.mongodb.core.*;
import org.springframework.data.mongodb.core.index.Index;
import org.springframework.data.mongodb.core.query.*;
import org.apache.log4j.Logger;
import com.victor.midas.model.*;

public class StockDao {
	private final String STOCKS_COLLECTION = "stocks"; 
	
	private final Logger logger = Logger.getLogger(StockDao.class);
	
	
	@Autowired  
    MongoTemplate mongoTemplate; 
	
	public void createCollection(){
		if (!mongoTemplate.collectionExists(STOCKS_COLLECTION)) {
			logger.info("mongoTemplate create collection");
            mongoTemplate.createCollection(STOCKS_COLLECTION);
            IndexOperations io = mongoTemplate.indexOps(STOCKS_COLLECTION);
            Index index =new Index();
            index.on("name", Direction.ASC);
            io.ensureIndex(index);
        }
	}
	
	public void dropCollection(){
		if (mongoTemplate.collectionExists(STOCKS_COLLECTION)) {
            mongoTemplate.dropCollection(STOCKS_COLLECTION);
        }
	}
	
	public void saveStock(Stock stock){
		mongoTemplate.save(stock, STOCKS_COLLECTION);
	}
	
	public Stock queryByName(String name){  
		return mongoTemplate.findOne(new Query(Criteria.where("name").is(name)), Stock.class, STOCKS_COLLECTION);     
    } 
	
	public List<Stock> queryAll(String name){  
		return mongoTemplate.findAll(Stock.class, STOCKS_COLLECTION);     
    } 
}
