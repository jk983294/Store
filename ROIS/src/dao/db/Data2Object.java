package dao.db;

import java.sql.ResultSet;
import java.sql.SQLException;
/**
 * �ṩ�����ݿ�ת����Java����Ľӿ�
 * @author Jersey
 * @date 2011-4-19
 * @param <T>
 */
public interface Data2Object<T> {
	public T DataMap(ResultSet rs) throws SQLException;
}
