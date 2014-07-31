package com.victor.midas.dao;


import com.mongodb.*;

import java.net.UnknownHostException;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.Set;
import java.util.regex.Pattern;

import org.bson.types.ObjectId;

import static java.util.concurrent.TimeUnit.SECONDS;

public class MongoDao {
	
	
	
	private MongoClient mongoClient = null;
	private DB db = null;
	private DBCollection coll = null;
	
	public MongoDao() throws Exception{
		// To directly connect to a single MongoDB server (note that this will not auto-discover the primary even
		// if it's a member of a replica set:
//		MongoClient mongoClient = new MongoClient();		// or		
//		MongoClient mongoClient = new MongoClient( "localhost" );		// or		
		mongoClient = new MongoClient( "127.0.0.1" , 27017 );
		// or, to connect to a replica set, with auto-discovery of the primary, supply a seed list of members
//		MongoClient mongoClient = new MongoClient(Arrays.asList(new ServerAddress("localhost", 27017), new ServerAddress("localhost", 27018)));
		
//		mongoClient.setWriteConcern(WriteConcern.JOURNALED);		//change write concern		
		db = mongoClient.getDB( "mydb" );
//		boolean auth = db.authenticate(myUserName, myPassword);
		
		coll = db.getCollection("testData1");
	}
	
	public void adminDemo() {
		for (String s : mongoClient.getDatabaseNames()) {
		   System.out.println(s);
		}
//		mongoClient.dropDatabase("databaseToBeDropped");
		//create Collection
		db.createCollection("testCollection", new BasicDBObject("capped", true).append("size", 1048576));
		
		for (String s : db.getCollectionNames()) {
			   System.out.println(s);
		}
		DBCollection testCollection = db.getCollection("testCollection");
		testCollection.drop();
		System.out.println(db.getCollectionNames());
	}
	
	public void indexDemo() {
		List<DBObject> list = coll.getIndexInfo();

		for (DBObject o : list) {
		   System.out.println(o.get("key"));
		}
		coll.createIndex(new BasicDBObject("i", 1));  // create index on "i", ascending
		for (DBObject o : list) {
			   System.out.println(o.get("key"));
			}
	}
	
	public void createAndInsert() {		
		BasicDBObject doc = new BasicDBObject("name", "MongoDB")
	        .append("type", "database").append("count", 1)
	        .append("info", new BasicDBObject("x", 203).append("y", 102));
		
		coll.insert(doc);
		
		DBObject myDoc = coll.findOne();
		System.out.println(myDoc);
		
		System.out.println("insert multi-docs : ");
		for (int i=0; i < 100; i++) {
		    coll.insert(new BasicDBObject("i", i));
		}
	}
	
	void queryDemo(){
		Set<String> colls = db.getCollectionNames();
		for (String s : colls) {
		    System.out.println(s);
		}	
		System.out.println(coll.getCount());
		
		DBCursor cursor = coll.find();
		try {
			System.out.println("use cursor to iterate docs : ");
		   while(cursor.hasNext()) {		// use cursor to iterate
		       System.out.println(cursor.next());
		   }
		} finally {
		   cursor.close();
		}
		
		BasicDBObject query = new BasicDBObject("i", 71);
		cursor = coll.find(query);			
		try {
			System.out.println("query single doc : ");
		   while(cursor.hasNext()) {
		       System.out.println(cursor.next());
		   }
		} finally {
		   cursor.close();
		}
		
		// find all where i > 50
		query = new BasicDBObject("i", new BasicDBObject("$gt", 90));
		cursor = coll.find(query);
		try {
			System.out.println("query all where i > 90 docs : ");
		    while (cursor.hasNext()) {
		        System.out.println(cursor.next());
		    }
		} finally {
		    cursor.close();
		}
		
		coll.find().maxTime(1, SECONDS).count(); 		//query over 1 second will abort
		// allow parallel cursor
		ParallelScanOptions parallelScanOptions = ParallelScanOptions
		        .builder().numCursors(3).batchSize(300).build();
		
		List<Cursor> cursors = coll.parallelScan(parallelScanOptions);
		for (Cursor pCursor: cursors) {
		    while (pCursor.hasNext()) {
		        System.out.println((pCursor.next()));
		    }
		}
	}
	
	public void bulkOperation() {
		// 1. Ordered bulk operation
		BulkWriteOperation builder = coll.initializeOrderedBulkOperation();
		builder.insert(new BasicDBObject("_id", 1));
		builder.insert(new BasicDBObject("_id", 2));
		builder.insert(new BasicDBObject("_id", 3));

		builder.find(new BasicDBObject("_id", 1)).updateOne(new BasicDBObject("$set", new BasicDBObject("x", 2)));
		builder.find(new BasicDBObject("_id", 2)).removeOne();
		builder.find(new BasicDBObject("_id", 3)).replaceOne(new BasicDBObject("_id", 3).append("x", 4));

		BulkWriteResult result = builder.execute();

		// 2. Unordered bulk operation - no guarantee of order of operation
		builder = coll.initializeUnorderedBulkOperation();
		builder.find(new BasicDBObject("_id", 1)).removeOne();
		builder.find(new BasicDBObject("_id", 2)).removeOne();

		result = builder.execute();
	}
	
	public void javaType() {
		ObjectId id = new ObjectId();
//		ObjectId copy = new ObjectId(id);
		
		Pattern john = Pattern.compile("joh?n", Pattern.CASE_INSENSITIVE);
		BasicDBObject query = new BasicDBObject("name", john);
		// finds all people with "name" matching /joh?n/i
		DBCursor cursor = coll.find(query);
		
		Date now = new Date();
		BasicDBObject time = new BasicDBObject("ts", now);
		coll.save(time);
		
		BasicDBObject y = new BasicDBObject("y", 3);
		BasicDBObject x = new BasicDBObject("x", y);		//	{ "x" : {  "y" : 3  }  }
		
		ArrayList arraydoc = new ArrayList();
		arraydoc.add(1);
		arraydoc.add(2);
		arraydoc.add(new BasicDBObject("foo", "bar"));
		arraydoc.add(4);

		BasicDBObject doc = new BasicDBObject("arraydoc", arraydoc);		//{  "x" : [ 1,2, {"foo" : "bar"}, 4 ] }
		
	}

	public static void main(String[] args) throws Exception {
		MongoDao mongoDao = new MongoDao();
//		mongoDao.createAndInsert();
//		mongoDao.queryDemo();
//		mongoDao.adminDemo();
//		mongoDao.indexDemo();
	}
}
