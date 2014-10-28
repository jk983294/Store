package com.victor.midas.dao;

import com.victor.midas.model.vo.StockVo;
import com.victor.midas.util.MidasConstants;
import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.mongodb.core.MongoTemplate;
import org.springframework.data.mongodb.core.query.Criteria;
import org.springframework.data.mongodb.core.query.Query;
import org.springframework.stereotype.Component;

import java.util.Collection;

/**
 * Dao for index collection
 */
@Component
public class StockDao {
    private static final String COLLECTION_NAME = MidasConstants.STOCK_COLLECTION_NAME;

    private static final Logger logger = Logger.getLogger(StockDao.class);

    @Autowired
    private MongoTemplate mongoTemplate;

    public StockVo queryStock(String stockName){
        return mongoTemplate.findOne(new Query(Criteria.where("_id").is(stockName)), StockVo.class, COLLECTION_NAME);
    }

    /**
     * save the task to DB, the task Id will be populated automatically
     * next time, if save again, it will saved by that Id
     */
    public void saveStock(Collection<StockVo> stocks){
        for (StockVo stock : stocks) {
            saveStock(stock);
        }
    }
    public void saveStock(StockVo stock){
        mongoTemplate.save(stock, COLLECTION_NAME);
    }

    /**
     * create task collection
     */
    public void createCollection(){
        if (!mongoTemplate.collectionExists(COLLECTION_NAME)) {
            logger.info("mongoTemplate create collection");
            mongoTemplate.createCollection(COLLECTION_NAME);
        }
    }

    /**
     * delete task collection, means that all task documents will be deleted
     */
    public void deleteCollection(){
        if (mongoTemplate.collectionExists(COLLECTION_NAME)) {
            logger.info("drop stock Collection");
            mongoTemplate.dropCollection(COLLECTION_NAME);
        }
    }

}
