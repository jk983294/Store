package com.sample.hsql;

/**
 * 数据库基本信息
 */
public class HSQLDBConfig {
	//数据库名称
	public static final String DBNAME = "bookDB";
	//数据库路径
	public static final String DBPATH = "WEB-INF/db/";
	//数据库端口
	public static final int DBPORT = 9001;
	//脚本文件名称
	public static final String SCRIPTFILE = DBPATH + DBNAME + ".script";
	//属性文件名称
	public static final String PROPERTIESFILE = DBPATH + DBNAME + ".properties";
	//URL
	public static final String URL = "jdbc:hsqldb:hsql://localhost:"+DBPORT+"/"+DBNAME;
	//用户名
	public static final String USERNAME = "sa";
	//密码
	public static final String PASSWORD = "";
	//驱动
	public static final String DRIVER = "org.hsqldb.jdbcDriver";
}
