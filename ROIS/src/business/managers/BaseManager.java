package business.managers;

import java.sql.SQLException;

import dao.db.Database;

import support.DatabaseManager;

/**
 * 提供CRUD功能DB支持的基类
 * @author Jersey
 * @date 2011-4-19
 */
public class BaseManager {

	public Database getDb() throws SQLException {
		Database db = null;

		db = DatabaseManager.getMySqlDatabase();

		return db;
	}
}
