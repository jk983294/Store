package com.victor.midas.services;

import javax.ws.rs.*;
import javax.ws.rs.core.MediaType;

import org.slf4j.*;
import org.springframework.beans.factory.annotation.Autowired;

import com.victor.midas.dao.StockDao;
import com.victor.midas.model.Welcome;
 

@Path("helloworld")
public class HelloRest {
	
	final Logger logger = LoggerFactory.getLogger(HelloRest.class);
	
	@Autowired
	StockDao stockdao;
	
	 @GET
	 @Produces(MediaType.TEXT_PLAIN)
	 public String ping() {
		 return "Hey, This is Jersey JAX-RS !";
	 }
	 
	 @GET
	 @Path("/hellojson")
	 @Produces(MediaType.APPLICATION_JSON)
	 public Welcome simplejson() {
		 Welcome welcome=new Welcome();
		 welcome.setDomain("www.javahash.com");
		 welcome.setMessage("JSON Example");
		 return welcome;
	 }
 
}
