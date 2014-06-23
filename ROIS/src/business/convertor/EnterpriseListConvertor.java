package business.convertor;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

import business.entityBeans.EnterpriseBean;

import dao.db.Data2Object;



public class EnterpriseListConvertor implements
Data2Object<List<EnterpriseBean>> {
@Override
public List<EnterpriseBean> DataMap(ResultSet rs) throws SQLException {
List<EnterpriseBean> entps = new ArrayList<EnterpriseBean>();
while (rs.next()) {
	EnterpriseBean entp = new EnterpriseBean();
	entp.setEid(rs.getString("eid"));
	entp.setEname(rs.getString("ename"));
	entp.setLoginName(rs.getString("loginName"));
	entp.setLoginPwd(rs.getString("loginPwd"));
	entp.setLocation(rs.getString("location"));
	entp.setEmail(rs.getString("email"));
	entp.setPhone(rs.getString("phone"));
	entps.add(entp);
}
return entps;
}

}