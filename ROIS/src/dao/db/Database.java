package dao.db;

import java.sql.SQLException;

public interface Database {
	int ExecuteNoneQuery(String cmdtext, Parameters parms) throws SQLException;

	<T> T ExecuteObject(Data2Object<T> convertor, String cmdtext,
			Parameters parms) throws SQLException;

	Object ExecuteScalar(String cmdtext, Parameters parms) throws SQLException;

	Parameters CreateFixedParms(int size);
}
