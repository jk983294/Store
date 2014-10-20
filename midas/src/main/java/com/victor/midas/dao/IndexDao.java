package com.victor.midas.dao;

import com.victor.midas.model.db.IndexDb;
import com.victor.midas.model.db.TaskDb;
import com.victor.midas.util.MidasConstants;
import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.domain.Sort;
import org.springframework.data.mongodb.core.IndexOperations;
import org.springframework.data.mongodb.core.MongoTemplate;
import org.springframework.data.mongodb.core.index.Index;
import org.springframework.data.mongodb.core.query.Criteria;
import org.springframework.data.mongodb.core.query.Query;
import org.springframework.stereotype.Component;

import java.util.Collection;
import java.util.List;

/**
 * Dao for index collection
 */
@Component
public class IndexDao {
    private static final String COLLECTION_NAME = MidasConstants.INDEX_COLLECTION_NAME;

    private static final Logger logger = Logger.getLogger(IndexDao.class);

    @Autowired
    private MongoTemplate mongoTemplate;

    public List<IndexDb> queryAllIndex(String stockName){
        return mongoTemplate.find(new Query(Criteria.where("_id").regex(stockName)), IndexDb.class, COLLECTION_NAME);
    }

    public IndexDb queryIndex(String indexName){
        return mongoTemplate.findOne(new Query(Criteria.where("_id").is(indexName)), IndexDb.class, COLLECTION_NAME);
    }

    /**
     * save the task to DB, the task Id will be populated automatically
     * next time, if save again, it will saved by that Id
     */
    public void saveIndex(Collection<IndexDb> indexDbs){
        for (IndexDb index : indexDbs) {
            saveIndex(index);
        }
    }
    public void saveIndex(IndexDb indexDb){
        mongoTemplate.save(indexDb, COLLECTION_NAME);
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
            logger.info("drop task Collection");
            mongoTemplate.dropCollection(COLLECTION_NAME);
        }
    }

}
