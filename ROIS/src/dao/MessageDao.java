package dao;

import java.sql.SQLException;
import java.util.List;

import business.entityBeans.MessageBean;

/**
 * ��ϢBean�����ݿ���ʽӿ�
 * @author Jersey
 * @date 2011-4-19
 */
public interface MessageDao {
	public int addMessage(MessageBean mb) throws SQLException;

	public List<MessageBean> queryByEid(String eid) throws SQLException;
	
	public int updateMessage(MessageBean mb)throws SQLException;
}
