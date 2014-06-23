package support;

import java.sql.SQLException;

import dao.db.Database;
import dao.db.MySqlDatabase;
/**
 * 数据访问模块的管理类
 * @author Jersey
 * @date 2011-4-19
 */
public abstract class DatabaseManager {
	/**
	 * 返回一个{@link Database}实例{@link MySqlDatabase}
	 * @return
	 * @throws SQLException
	 */
	public static Database getMySqlDatabase() throws SQLException {
		return new MySqlDatabase();
	}
}
