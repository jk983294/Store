package dao;

import java.sql.SQLException;
import java.util.List;

import business.entityBeans.EnterpriseBean;
import business.entityBeans.ServiceBean;
import business.entityBeans.SrvEntpBean;

/**
 * 服务Bean的数据库访问接口
 * @author Jersey
 * @date 2011-4-19
 */
public interface ServiceDao {

	public int registerService(SrvEntpBean se) throws SQLException;

	public ServiceBean queryByName(String sname) throws SQLException;

	public ServiceBean queryBySid(String sid) throws SQLException;

	public List<ServiceBean> queryAllServices() throws SQLException;

	public List<EnterpriseBean> queryEntpBySid(String sid) throws SQLException;

	public List<ServiceBean> querySrvcByEid(String eid) throws SQLException;

	public List<SrvEntpBean> querySrvEntpByEid(String eid) throws SQLException;

	public List<SrvEntpBean> querySrvEntpBySid(String Sid) throws SQLException;

	public int addServices(List<ServiceBean> list) throws SQLException;

	SrvEntpBean querySrvEntpByEidSid(String eid, String sid)
			throws SQLException;

	int deleteSrvEntp(SrvEntpBean seb) throws SQLException;
}
