package dao.db;

import java.sql.ResultSet;
import java.sql.SQLException;
/**
 * 提供从数据库转换到Java对象的接口
 * @author Jersey
 * @date 2011-4-19
 * @param <T>
 */
public interface Data2Object<T> {
	public T DataMap(ResultSet rs) throws SQLException;
}
