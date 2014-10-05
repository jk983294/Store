package com.victor.midas.dao;

import java.util.List;

import com.victor.midas.model.db.TaskDb;
import com.victor.midas.util.MidasConstants;
import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.domain.Sort.Direction;
import org.springframework.data.mongodb.core.IndexOperations;
import org.springframework.data.mongodb.core.MongoTemplate;
import org.springframework.data.mongodb.core.index.Index;
import org.springframework.data.mongodb.core.query.Query;
import org.springframework.stereotype.Component;


@Component
public class TaskDao {
	private final static String COLLECTION_NAME = MidasConstants.TASK_COLLECTION_NAME;
	
	private final static Logger logger = Logger.getLogger(TaskDao.class);
	
	@Autowired  
    private MongoTemplate mongoTemplate; 
	
	/**
	 * get latest N task
	 */
	public List<TaskDb> queryLastTasks(int n){
		Query query =new Query().withHint("submit_-1").limit(n);
        return mongoTemplate.find(query, TaskDb.class, COLLECTION_NAME);
	}
	
	/**
	 * save the task to DB, the task Id will be populated automatically
	 * next time, if save again, it will saved by that Id
	 */
	public void saveTask(TaskDb taskDb){
		mongoTemplate.save(taskDb, COLLECTION_NAME);
	}
	
	/**
	 * create task collection
	 */
	public void createCollection(){
		if (!mongoTemplate.collectionExists(COLLECTION_NAME)) {
			logger.info("mongoTemplate create collection");
            mongoTemplate.createCollection(COLLECTION_NAME);
            IndexOperations io = mongoTemplate.indexOps(COLLECTION_NAME);
            Index index =new Index();
            index.on("submit", Direction.DESC);
            io.ensureIndex(index);
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
