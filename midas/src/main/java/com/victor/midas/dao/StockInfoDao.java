package com.victor.midas.dao;

import java.util.List;

import com.victor.midas.model.db.StockInfoDb;
import com.victor.midas.model.vo.StockVo;
import com.victor.midas.util.MidasConstants;
import org.springframework.beans.factory.annotation.*;
import org.springframework.data.domain.Sort.Direction;
import org.springframework.data.mongodb.core.*;
import org.springframework.data.mongodb.core.index.Index;
import org.springframework.data.mongodb.core.query.*;
import org.apache.log4j.Logger;

import org.springframework.stereotype.Component;

@Component
public class StockInfoDao {
	private final String COLLECTION_NAME = MidasConstants.STOCKINFO_COLLECTION_NAME;
	
	private static final Logger logger = Logger.getLogger(StockInfoDao.class);
	
	
	@Autowired  
    MongoTemplate mongoTemplate; 

	/**
	 * save stock, cause its name is Id in MongoDB, so it should have name field first
	 */
    public void saveStockInfo(List<StockInfoDb> stocks){
        for (StockInfoDb stockInfo : stocks){
            saveStockInfo(stockInfo);
        }
    }
	public void saveStockInfo(StockInfoDb stockInfo){
		mongoTemplate.save(stockInfo, COLLECTION_NAME);
	}
	
	/**
	 * query one stock by its name
	 */
	public StockInfoDb queryByName(String stockName){
		return mongoTemplate.findOne(new Query(Criteria.where("_id").is(stockName)), StockInfoDb.class, COLLECTION_NAME);
    }

    public List<StockInfoDb> queryAllBasicInfo(){
        return mongoTemplate.findAll(StockInfoDb.class, COLLECTION_NAME);
    }

    /**
	 * for paginate, skip first records, return only limit records
     */
    public List<StockVo> getStockByPaging(int first, int end) {
    	Query query =new Query().skip(first).limit(end);
		query.fields().include("name").include("desp").include("latest");
        return mongoTemplate.find(query, StockVo.class, COLLECTION_NAME);
    }
	

	
	public List<StockVo> queryAllName4AutoCompletion(){
		Query query =new Query();
		query.fields().include("name");
		return mongoTemplate.find(query,StockVo.class, COLLECTION_NAME);
    } 
	
	public int getStockCount() {
        return (int) mongoTemplate.count( new Query(), COLLECTION_NAME);
    }


    /**
     * create collection
     */
    public void createCollection(){
        if (!mongoTemplate.collectionExists(COLLECTION_NAME)) {
            logger.info("mongoTemplate create collection");
            mongoTemplate.createCollection(COLLECTION_NAME);
        }
//        if (!mongoTemplate.collectionExists(COLLECTION_NAME)) {
//            logger.info("mongoTemplate create collection");
//            mongoTemplate.createCollection(COLLECTION_NAME);
//            IndexOperations io = mongoTemplate.indexOps(COLLECTION_NAME);
//            Index index =new Index();
//            index.on("latest.change", Direction.DESC);
//            io.ensureIndex(index);
//        }
    }

    /**
     * delete collection, means that all documents will be deleted
     */
    public void deleteCollection(){
        if (mongoTemplate.collectionExists(COLLECTION_NAME)) {
            logger.info("drop task Collection");
            mongoTemplate.dropCollection(COLLECTION_NAME);
        }
    }
}
