package business.managers;


import java.sql.SQLException;
import java.util.List;

import dao.EnterpriseDao;
import dao.db.DBType;
import dao.db.MySqlParameter;
import dao.db.Parameters;


import business.convertor.EnterpriseListConvertor;
import business.entityBeans.EnterpriseBean;

/**
 * 针对TEnterprises表的增删改查的管理类
 * @author Jersey
 * @date 2011-4-19
 */
public class EnterpriseManager extends BaseManager implements EnterpriseDao {

	public EnterpriseManager() throws SQLException {
		super();
	}

	/**
	 * Check whether the user is valid, if there is no such user, return "NONE",
	 * if the password is incorrect, return "WRONG", if the user is valid return
	 * EID
	 * 
	 * @param loginName
	 * @param pwd
	 * @return
	 * @throws SQLException
	 */
	public String checkLogin(String loginName, String pwd) throws SQLException {
		EnterpriseBean eb = this.queryByLoginName(loginName);
		if (eb == null)
			return "NONE";
		else {
			if (eb.getLoginPwd().equals(pwd))
				return eb.getEid();
			else
				return "WRONG";
		}
	}

	public int addEntp(EnterpriseBean eb) throws SQLException {
		String sql = "INSERT INTO TEnterprises "
				+ "(ename,loginName,loginPwd,location,email,phone,edescription)"
				+ "VALUES(?,?,?,?,?,?,?)";
		Parameters params = getDb().CreateFixedParms(7);
		params.add(new MySqlParameter(DBType.String, eb.getEname()));
		params.add(new MySqlParameter(DBType.String, eb.getLoginName()));
		params.add(new MySqlParameter(DBType.String, eb.getLoginPwd()));
		params.add(new MySqlParameter(DBType.String, eb.getLocation()));
		params.add(new MySqlParameter(DBType.String, eb.getEmail()));
		params.add(new MySqlParameter(DBType.String, eb.getPhone()));
		params.add(new MySqlParameter(DBType.String, eb.getDesc()));
		return getDb().ExecuteNoneQuery(sql, params);
	}

	public List<EnterpriseBean> queryAllEntp() throws SQLException {
		String sql = "SELECT * FROM TEnterprises";
		return getDb().ExecuteObject(new EnterpriseListConvertor(), sql, null);
	}

	

	@Override
	public int deleteEntp(EnterpriseBean eb) throws SQLException {
		String sql = "DELETE FROM TENTERPRISES WHERE EID=?";
		Parameters params = getDb().CreateFixedParms(1);
		params.add(new MySqlParameter(DBType.Int32, eb.getEid()));
		return getDb().ExecuteNoneQuery(sql, params);
	}

	@Override
	public int updateEntp(EnterpriseBean eb) throws SQLException {
		String sql = "UPDATE TENTERPRISES "
				+ "SET ename=?,loginName=?,loginPwd=?,"
				+ "location=?,email=?,phone=?,edescription=? " + "WHERE EID=?";
		Parameters params = getDb().CreateFixedParms(8);
		params.add(new MySqlParameter(DBType.String, eb.getEname()));
		params.add(new MySqlParameter(DBType.String, eb.getLoginName()));
		params.add(new MySqlParameter(DBType.String, eb.getLoginPwd()));
		params.add(new MySqlParameter(DBType.String, eb.getLocation()));
		params.add(new MySqlParameter(DBType.String, eb.getEmail()));
		params.add(new MySqlParameter(DBType.String, eb.getPhone()));
		params.add(new MySqlParameter(DBType.String, eb.getDesc()));
		params.add(new MySqlParameter(DBType.Int32, eb.getEid()));
		return getDb().ExecuteNoneQuery(sql, params);
	}

	@Override
	public EnterpriseBean queryByEname(String name) throws SQLException {
		String sql = "SELECT * FROM TENTERPRISES WHERE ENAME=?";
		Parameters params = getDb().CreateFixedParms(1);
		params.add(new MySqlParameter(DBType.String, name));
		List<EnterpriseBean> entps = getDb().ExecuteObject(
				new EnterpriseListConvertor(), sql, params);
		
		return entps.size()==0?null:entps.get(0);
	}

	@Override
	public EnterpriseBean queryByLoginName(String loginName)
			throws SQLException {
		String sql = "SELECT * FROM TENTERPRISES WHERE loginName=?";
		Parameters params = getDb().CreateFixedParms(1);
		params.add(new MySqlParameter(DBType.String, loginName));
		List<EnterpriseBean> entps = getDb().ExecuteObject(
				new EnterpriseListConvertor(), sql, params);
		if (entps.size()==0)
			return null;
		return entps.get(0);
	}
}
