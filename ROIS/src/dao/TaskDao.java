package dao;

import java.sql.SQLException;
import java.util.List;

import business.entityBeans.TaskBean;
import business.entityBeans.TaskEntpBean;

import support.TaskState;
/**
 * ����Bean�����ݿ���ʽӿ�
 * @author Jersey
 * @date 2011-4-19
 */
public interface TaskDao {
	public int addTask(TaskBean tb) throws SQLException;

	public int deleteTask(TaskBean tb) throws SQLException;
	
	public List<TaskBean> checkBidDeadline() throws SQLException;
	public int biddingTask(TaskEntpBean te) throws SQLException;

	public List<TaskBean> querySubTaskByEid(String eid) throws SQLException;

	public List<TaskEntpBean> queryTaskEntpByEid(String eid) throws SQLException;

	public List<TaskEntpBean> queryTaskEntpByTid(String tid) throws SQLException;
	
	/**
	 * ����EID���ش���sanme�ֶε������б�������ҵ�Ѿ���õ���������ھ��������
	 * @param eid
	 * @return
	 * @throws SQLException
	 */
	public List<TaskBean> queryMyTaskByEid(String eid) throws SQLException;

	public TaskBean queryTaskByTid(String tid) throws SQLException;

	public int updateTaskState(String tid, TaskState state) throws SQLException;

	/**
	 * ���¶�ӦTID��task��eid�ֶκ�state�ֶ�
	 * @param tid
	 * @param eid
	 * @return
	 * @throws SQLException
	 */
	public int updateTaskOnwer(String tid, String eid) throws SQLException;

	public int DeleteBidding(String tid) throws SQLException;

}
