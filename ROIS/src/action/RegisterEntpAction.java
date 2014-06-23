package action;

import java.sql.SQLException;

import business.entityBeans.EnterpriseBean;
import business.managers.EnterpriseManager;

import com.opensymphony.xwork2.ActionSupport;


public class RegisterEntpAction extends ActionSupport {

	private static final long serialVersionUID = -988390834893339561L;
	private EnterpriseBean enterprise;

	public void setEnterprise(EnterpriseBean enterprise) {
		this.enterprise = enterprise;
	}

	public EnterpriseBean getEnterprise() {
		return enterprise;
	}

	@Override
	public String execute() {
		try {
			EnterpriseManager em=new EnterpriseManager();
			if(em.queryByEname(enterprise.getEname())!=null){
				this.addFieldError("eName", "����ҵ�����Ѵ��ڣ����������롣");
			}
			if(em.queryByLoginName(enterprise.getLoginName())!=null){
				this.addFieldError("eName", "�õ�¼���Ѵ��ڣ����������롣");
			}
			if(this.hasErrors())
				return INPUT;
			if(em.addEntp(enterprise)<=0)
				return ERROR;
		} catch (SQLException e) {
			return ERROR;
		}
		
		return SUCCESS;
	}

	@Override
	public void validate() {
		//Other validation has already been done in validation.xml
		if(null==getRepassword() ||!getRepassword().equals(enterprise.getLoginPwd()))
			this.addFieldError("repassword", "������������벻һ��������������.");
	}

	private String repassword;

	public void setRepassword(String repassword) {
		this.repassword = repassword;
	}

	public String getRepassword() {
		return repassword;
	}
}
