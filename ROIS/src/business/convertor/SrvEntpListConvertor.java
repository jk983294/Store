package business.convertor;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

import business.entityBeans.SrvEntpBean;

import dao.db.Data2Object;


public class SrvEntpListConvertor implements Data2Object<List<SrvEntpBean>>{

	@Override
	public List<SrvEntpBean> DataMap(ResultSet rs) throws SQLException {
		List<SrvEntpBean> list=new ArrayList<SrvEntpBean>();
		while(rs.next()){
			SrvEntpBean seb=new SrvEntpBean();
			seb.setSid(rs.getString("sid"));
			try{
				seb.setSname(rs.getString("sname"));
			}catch(SQLException e){
				//SNAME还是一次性联表查出来的好……虽然很丑陋……
			}
			seb.setEid(rs.getString("eid"));
			seb.setUrl(rs.getString("entpSrvcXML"));
			list.add(seb);
		}
		return list;
	}
	

}
