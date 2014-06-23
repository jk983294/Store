package business.convertor;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

import business.entityBeans.ServiceBean;

import dao.db.Data2Object;



public class ServiceListConvertor implements Data2Object<List<ServiceBean>> {

	@Override
	public List<ServiceBean> DataMap(ResultSet rs) throws SQLException {
		List<ServiceBean> list = new ArrayList<ServiceBean>();
		while (rs.next()) {
			ServiceBean sb = new ServiceBean();
			sb.setSid(rs.getString("SID"));
			sb.setSname(rs.getString("SNAME"));
			sb.setUrl(rs.getString("ServiceParamXml"));
			list.add(sb);
		}
		return list;
	}
}
