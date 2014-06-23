package business.convertor;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

import business.entityBeans.TaskBean;

import dao.db.Data2Object;
/**
 * ת�����ݿ��е�Task Row SetΪList<TaskBean>
 * @author Jersey
 * @date 2011-4-19
 */
public class TaskListConvertor implements Data2Object<List<TaskBean>> {

	@Override
	public List<TaskBean> DataMap(ResultSet rs) throws SQLException {
		List<TaskBean> list = new ArrayList<TaskBean>();
		while (rs.next()) {
			TaskBean tb = new TaskBean();

			tb.setTid(rs.getString("TID"));
			tb.setSid(rs.getString("SID"));
			try {
				tb.setSname(rs.getString("sname"));
			} catch (SQLException e) {
				// ��ΪSNAME��Ҫ�����ѯ��������ʱ���ǲ����
				// ���sname������Ϊ�˷�����ʾ�㡭��
			}
			tb.setEid(rs.getString("EID"));
			tb.setPubTime(rs.getTimestamp("PUBTIME"));
			tb.setStartTime(rs.getTimestamp("STARTTIME"));
			tb.setEndTime(rs.getTimestamp("ENDTIME"));
			tb.setDeadlineTime(rs.getTimestamp("DEADLINETIME"));
			tb.setBidDeadline(rs.getTimestamp("BIDDEADLINE"));
			tb.setState(rs.getString("STATE"));
			tb.setUrl(rs.getString("PARAMXML"));

			list.add(tb);
		}
		return list;
	}

}