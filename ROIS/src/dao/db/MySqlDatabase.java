package dao.db;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Timestamp;

import javax.naming.Context;
import javax.naming.InitialContext;
import javax.naming.NamingException;
import javax.sql.DataSource;

/**
 * Mysql���ݿ��JDBCʵ��
 * 
 * @author Jersey
 * @date 2011-4-19
 */
public class MySqlDatabase implements Database {
	private static final int BATCH_LIMITE=1000;
	private Connection conn;
	static private DataSource ds;
	static {
		try {
			Context context = new InitialContext();
			ds = (DataSource) context.lookup("java:comp/env/jdbc/mysql");
		} catch (NamingException e) {
			System.err.print(e.getMessage());
		}
	}

	public MySqlDatabase() throws SQLException {

		conn = ds.getConnection();
	}
	private int calculateSum(int[] array){
		int sum=0;
		for(int i=0;i<array.length;i++){
			sum+=array[i];
		}
		return sum;
	}
	/**
	 * ִ�зǲ�ѯ��SQL������Ӱ�������
	 */
	public int ExecuteNoneQuery(String cmdtext, Parameters parms)
			throws SQLException {
		PreparedStatement pstmt = null;
		try {
			pstmt = conn.prepareStatement(cmdtext);
			if (parms.needBatch()) {
				//�����Ҫִ��������
				int[] result;
				int affectedLine=0;
				for (int i = 0; i < parms.getLength(); i++) {
					prepareCommand(pstmt, (Parameters)parms.getParameter(i));
					pstmt.addBatch();
					if((i+1)%BATCH_LIMITE==0){
						result=pstmt.executeBatch();
						affectedLine+=calculateSum(result);
						conn.commit();
						pstmt.clearBatch();
					}
					result=pstmt.executeBatch();
					affectedLine+=calculateSum(result);
				}
				return affectedLine;
			} else {
				//����Ҫִ��������
				prepareCommand(pstmt, parms);
				return pstmt.executeUpdate();
			}
		} catch (Exception ex) {
			System.err.println(ex.getMessage());
		} finally {
			if (pstmt != null) {
				pstmt.clearParameters();
				pstmt.close();
			}
			if (conn != null)
				conn.close();
		}
		return -1;
	}


	/**
	 * ִ�в�ѯ��SQL���ض���
	 */
	public <T> T ExecuteObject(Data2Object<T> convertor, String cmdtext,
			Parameters parms) throws SQLException {
		PreparedStatement pstmt = null;
		ResultSet rs = null;
		try {
			pstmt = conn.prepareStatement(cmdtext);

			prepareCommand(pstmt, parms);
			rs = pstmt.executeQuery();

			return convertor.DataMap(rs);
		} catch (Exception ex) {
		} finally {
			if (rs != null)
				rs.close();
			if (pstmt != null)
				pstmt.close();
			if (conn != null)
				conn.close();
		}
		return null;
	}

	/**
	 * ִ�в�ѯ���ص�һ������
	 */
	public Object ExecuteScalar(String cmdtext, Parameters parms)
			throws SQLException {
		PreparedStatement pstmt = null;
		ResultSet rs = null;
		try {
			pstmt = conn.prepareStatement(cmdtext);
			prepareCommand(pstmt, parms);

			rs = pstmt.executeQuery();
			if (rs.next()) {
				return rs;
			} else {
				return null;
			}
		} catch (Exception e) {
		} finally {
			if (rs != null)
				rs.close();
			if (pstmt != null)
				pstmt.close();
			if (conn != null)
				conn.close();
		}
		return null;
	}

	/**
	 * ׼��SQL��䣬�ռλ��
	 * 
	 * @param pstmt
	 * @param parms
	 * @throws SQLException
	 */
	private void prepareCommand(PreparedStatement pstmt, Parameters parms)
			throws SQLException {
		if (parms != null && parms.getLength() > 0) {
			for (int i = 0; i < parms.getLength(); i++) {
				MySqlParameter parm = (MySqlParameter) parms.getParameter(i);
				// �����null�Ļ���������
				if (parm.getValue() == null) {
					pstmt.setObject(i + 1, null);// setObject�����ǿ�󰡣����Կ���ȫ����setObject��
					continue;
				}
				String value = String.valueOf(parm.getValue());
				switch (parm.getType()) {
				case String:
					pstmt.setString(i + 1, value);
					break;
				case Int16:
					pstmt.setShort(i + 1, Short.parseShort(value));
					break;
				case Int32:
					pstmt.setInt(i + 1, Integer.parseInt(value));
					break;
				case Int64:
					pstmt.setLong(i + 1, Long.parseLong(value));
					break;
				case DateTime:
					pstmt.setTimestamp(i + 1, Timestamp.valueOf(value));
					break;
				case Boolean:
					pstmt.setBoolean(i + 1, Boolean.valueOf(value));
					break;
				default:
					pstmt.setObject(i + 1, value);
					break;
				}
			}
		}
	}

	public Parameters CreateFixedParms(int size) {
		return new FixedParameters(size);
	}
}
