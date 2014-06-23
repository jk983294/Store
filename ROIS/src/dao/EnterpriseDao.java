package dao;

import java.sql.SQLException;

import business.entityBeans.EnterpriseBean;

/**
 * ��ҵ�û�Bean�����ݷ��ʽӿ�
 * @author Jersey
 * @date 2011-4-19
 */
public interface EnterpriseDao {
	public int addEntp(EnterpriseBean eb) throws SQLException;

	public int deleteEntp(EnterpriseBean eb) throws SQLException;

	public int updateEntp(EnterpriseBean eb) throws SQLException;
	
	public EnterpriseBean queryByEname(String name)throws SQLException;
	
	public EnterpriseBean queryByLoginName(String loginName)throws SQLException;
	
}