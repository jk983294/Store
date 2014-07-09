package com.sample.ext4.simplebook.util;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;

import org.apache.log4j.Logger;

import com.sample.hsql.HSQLDBConfig;

public class MyDBUtil {
	/**
	 * Logger for this class
	 */
	private static final Logger logger = Logger.getLogger(MyDBUtil.class);
	
	static{
		try {
			Class.forName(HSQLDBConfig.DRIVER);
		} catch (ClassNotFoundException e) {
			logger.error(e,e);
		}
	}
	/**
	 * 获取数据库连接对象
	 * @return 数据库连接对象
	 * @throws ClassNotFoundException
	 * @throws SQLException
	 */
	public static Connection getConnection() throws SQLException {
		return DriverManager.getConnection(HSQLDBConfig.URL, HSQLDBConfig.USERNAME, HSQLDBConfig.PASSWORD);
	}
}