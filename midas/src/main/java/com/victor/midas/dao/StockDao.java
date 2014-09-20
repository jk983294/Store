package com.victor.midas.dao;

import java.util.List;

import org.springframework.beans.factory.annotation.*;
import org.springframework.data.domain.Sort.Direction;
import org.springframework.data.mongodb.core.*;
import org.springframework.data.mongodb.core.index.Index;
import org.springframework.data.mongodb.core.query.*;
import org.apache.log4j.Logger;

import com.victor.midas.calculator.IndexCalculator;
import com.victor.midas.model.*;

public class StockDao {
	private final String STOCKS_COLLECTION = "stocks"; 
	
	private static final Logger logger = Logger.getLogger(StockDao.class);
	
	private final IndexCalculator calculator = new IndexCalculator();
	
	
	@Autowired  
    MongoTemplate mongoTemplate; 

	/**
	 * save stock, cause its name is Id in MongoDB, so it should have name field first
	 * @param stock
	 */
	public void saveStock(Stock stock){
		if (stock != null && stock.getRecords()!= null && stock.getRecords().size() > 0) {
			calculator.calculate(stock);
			stock.setLatest(stock.getRecords().get(stock.getRecords().size()-1));
		}
		mongoTemplate.save(stock, STOCKS_COLLECTION);
	}
	
	/**
	 * create stock collection
	 */
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
	
	/**
	 * delete stock collection, means that all stock documents will be deleted
	 */
	public void dropStockCollection(){
		if (mongoTemplate.collectionExists(STOCKS_COLLECTION)) {
			logger.info("drop Stock Collection");
            mongoTemplate.dropCollection(STOCKS_COLLECTION);
        }
	}
	
	/**
	 * query one stock by its name
	 * @param name
	 * @return
	 */
	public Stock queryByName(String name){  
		return mongoTemplate.findOne(new Query(Criteria.where("_id").is(name)), Stock.class, STOCKS_COLLECTION);     
    } 
	
	/**
	 * for paginate, skip first records, return only limit records
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
