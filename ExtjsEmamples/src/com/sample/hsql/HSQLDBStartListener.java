package com.sample.hsql;

import java.io.File;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.Statement;

import javax.servlet.ServletContext;
import javax.servlet.ServletContextEvent;
import javax.servlet.ServletContextListener;

import org.apache.commons.dbutils.DbUtils;
import org.apache.log4j.Logger;
import org.hsqldb.Server;

public class HSQLDBStartListener implements ServletContextListener {
	/**
	 * Logger for this class
	 */
	private static final Logger logger = Logger.getLogger(HSQLDBStartListener.class);
	
	public void contextInitialized(ServletContextEvent sce) {
		
		ServletContext ctx = sce.getServletContext();
		String realPath = ctx.getRealPath("/");
		
		File dbDir = new File(realPath + HSQLDBConfig.DBPATH);
		if (!dbDir.exists()) {
			if (!dbDir.mkdirs()) {
				logger.debug("创建数据库路径失败:" + dbDir);
				return;
			}
		}
		//脚本文件路径
		File scriptFile = new File(realPath + HSQLDBConfig.SCRIPTFILE);
		//属性文件路径
		File propertiesFile = new File(realPath + HSQLDBConfig.PROPERTIESFILE);
		if (scriptFile.exists() && propertiesFile.exists()) {
			Server server = new Server();
			server.setDatabaseName(0, HSQLDBConfig.DBNAME);
			server.setDatabasePath(0, realPath + HSQLDBConfig.DBPATH + HSQLDBConfig.DBNAME);
			server.setPort(HSQLDBConfig.DBPORT);
			server.setSilent(true);
			server.start();
			logger.debug("数据库启动成功...");
		} else {
			logger.debug("数据库启动失败！");
		}
	}

	/**
	 * Listener 销毁方法，在 Web 应用终止的时候执行"shutdown"命令关闭数据库.
	 */
	public void contextDestroyed(ServletContextEvent sce) {
		Connection conn = null;
		Statement stmt = null;
		try {
			Class.forName(HSQLDBConfig.DRIVER);
			conn = DriverManager.getConnection(HSQLDBConfig.URL, HSQLDBConfig.USERNAME, HSQLDBConfig.PASSWORD);
			stmt = conn.createStatement();
			stmt.executeUpdate("SHUTDOWN;");
		} catch (Exception e) {
			logger.error(e,e);
		} finally{
			DbUtils.closeQuietly(stmt);
			DbUtils.closeQuietly(conn);
		}
	}
}