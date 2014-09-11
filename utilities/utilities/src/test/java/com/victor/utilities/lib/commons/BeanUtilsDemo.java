package com.victor.utilities.lib.commons;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.commons.beanutils.BeanUtils;
import org.apache.commons.beanutils.ConstructorUtils;
import org.apache.commons.beanutils.MethodUtils;
import org.apache.commons.beanutils.PropertyUtils;
import org.apache.commons.math3.analysis.solvers.NewtonRaphsonSolver;

public class BeanUtilsDemo {
	
	public static void main(String[] args) throws Exception {
		DemoPojo demoPojo = new DemoPojo("jk", 12);	
		
		Map<String, String> values = new HashMap<>();
		values.put("1","234-222-1222211");
		values.put("2","021-086-1232323");
		
		List<String> list = new ArrayList<String>();
		list.add("foo");
		list.add("bar");
		
		PropertyUtils.setProperty(demoPojo,"values",values);
		BeanUtils.setProperty(demoPojo,"list",list);
		System.out.println(BeanUtils.getProperty(demoPojo, "a"));
		System.out.println(BeanUtils.getProperty(demoPojo, "values(2)"));	//values.get("2")
		System.out.println(BeanUtils.getMappedProperty(demoPojo, "values", "2"));	//values.get("2")
		System.out.println(BeanUtils.getProperty(demoPojo, "list[1]"));
		
		DemoPojo demoPojo2 = new DemoPojo();
	    BeanUtils.copyProperties(demoPojo2, demoPojo);		//shallow copy
	    System.out.println(demoPojo2.toString());
	    
	    /**
	     * dynamic all function
	     */
	    MethodUtils.invokeMethod(demoPojo2, "print", "asdf");
	    
	    
	    /**
	     * Dynamic construct object
	     */
	    Object[] ctorargs = new Object[2];
	    ctorargs[0] = new String("aha");
	    ctorargs[1] = new Integer(1024);
	    DemoPojo ahaObject = ConstructorUtils.invokeConstructor(DemoPojo.class, ctorargs);
	    System.out.println(ahaObject.toString());
	}
}

