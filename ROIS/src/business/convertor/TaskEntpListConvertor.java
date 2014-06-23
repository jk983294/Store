package business.convertor;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

import business.entityBeans.TaskEntpBean;

import dao.db.Data2Object;


public class TaskEntpListConvertor implements Data2Object<List<TaskEntpBean>>{

	@Override
	public List<TaskEntpBean> DataMap(ResultSet rs) throws SQLException {
		List<TaskEntpBean> list=new ArrayList<TaskEntpBean>();
		while(rs.next()){
			TaskEntpBean te=new TaskEntpBean();
			te.setTid(rs.getString("tid"));
			te.setEid(rs.getString("eid"));
			te.setBidTime(rs.getTimestamp("bidTime"));
			list.add(te);
		}
		return list;
	}

}
