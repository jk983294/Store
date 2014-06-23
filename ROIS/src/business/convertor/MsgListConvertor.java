package business.convertor;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

import business.entityBeans.MessageBean;

import dao.db.Data2Object;



public class MsgListConvertor implements Data2Object<List<MessageBean>>{

	@Override
	public List<MessageBean> DataMap(ResultSet rs) throws SQLException {
		List<MessageBean> list = new ArrayList<MessageBean>();
		while(rs.next()){
			MessageBean mb=new MessageBean();
			mb.setMid(rs.getString("MID"));
			mb.setEid(rs.getString("EID"));
			mb.setType(rs.getString("TYPE"));
			mb.setContent(rs.getString("CONTENT"));
			mb.setViewed(rs.getBoolean("VIEWED"));
			list.add(mb);
		}
		return list;
	}

	
}
