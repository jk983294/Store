package support;

import java.sql.SQLException;

import dao.db.Database;
import dao.db.MySqlDatabase;
/**
 * ���ݷ���ģ��Ĺ�����
 * @author Jersey
 * @date 2011-4-19
 */
public abstract class DatabaseManager {
	/**
	 * ����һ��{@link Database}ʵ��{@link MySqlDatabase}
	 * @return
	 * @throws SQLException
	 */
	public static Database getMySqlDatabase() throws SQLException {
		return new MySqlDatabase();
	}
}
