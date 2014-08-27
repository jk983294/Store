package com.victor.midas.dao;

import java.util.List;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.domain.Sort.Direction;
import org.springframework.data.mongodb.core.IndexOperations;
import org.springframework.data.mongodb.core.MongoTemplate;
import org.springframework.data.mongodb.core.index.Index;
import org.springframework.data.mongodb.core.query.Query;

import com.victor.midas.model.*;

public class TaskDao {
	private final String TASKS_COLLECTION = "tasks"; 
	
	private final Logger logger = Logger.getLogger(TaskDao.class);
	
	@Autowired  
    private MongoTemplate mongoTemplate; 
	
	/**
	 * get latest N task
	 * @param n
	 * @return
	 */
	public List<Task> lastTasks(int n){
		Query query =new Query().withHint("submit_-1").limit(n);
        return mongoTemplate.find(query, Task.class, TASKS_COLLECTION);
	}
	
	/**
	 * save the task to DB, the task Id will be populated automatically
	 * next time, if save again, it will saved by that Id
	 * @param task
	 */
	public void saveTask(Task task){
		mongoTemplate.save(task, TASKS_COLLECTION);
	}
	
	/**
	 * create stock and task collection
	 */
	public void createCollection(){
		if (!mongoTemplate.collectionExists(TASKS_COLLECTION)) {
			logger.info("mongoTemplate create collection");
            mongoTemplate.createCollection(TASKS_COLLECTION);
            IndexOperations io = mongoTemplate.indexOps(TASKS_COLLECTION);
            Index index =new Index();
            index.on("submit", Direction.DESC);
            io.ensureIndex(index);
        }
	}
	
	/**
	 * delete task collection, means that all task documents will be deleted
	 */
	public void dropTaskCollection(){
		if (mongoTemplate.collectionExists(TASKS_COLLECTION)) {
			logger.info("drop task Collection");
            mongoTemplate.dropCollection(TASKS_COLLECTION);
        }
	}
	
}