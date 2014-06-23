package action;

import java.sql.SQLException;

import business.managers.EnterpriseManager;

public class LoginEntpAction extends BaseAction {

	private static final long serialVersionUID = 7730978193107862806L;
	private String loginName;
	private String loginPwd;

	public String getLoginName() {
		return loginName;
	}

	public void setLoginName(String loginName) {
		this.loginName = loginName;
	}

	public String getLoginPwd() {
		return loginPwd;
	}

	public void setLoginPwd(String loginPwd) {
		this.loginPwd = loginPwd;
	}
	public String login(){
		if(!validateLogin())
			return INPUT;
		try {
			EnterpriseManager em = new EnterpriseManager();
			String eid = em.checkLogin(loginName, loginPwd);
			if ("NONE".equals(eid)) {
				this.addFieldError("loginName", "不存在此用户名！");
				return INPUT;
			} else if ("WRONG".equals(eid)) {
				this.addFieldError("loginPwd", "密码错误！");
				return INPUT;
			} else { // the user is valid
				getSession().put("eid", eid);
				getSession().put("loginName", loginName);
				return SUCCESS;
			}
		} catch (SQLException ex) {
			return ERROR;
		}
	}

	public String logout(){
		getSession().clear();
		return SUCCESS;
	}
	
	public boolean validateLogin() {
		boolean valid=true;
		if (null == getLoginName() || getLoginName().trim().equals("")){
			this.addFieldError("loginName", "请输入登录用户名。");
			valid=false;
		}
		if (null == getLoginPwd() || getLoginPwd().trim().equals("")){
			this.addFieldError("loginPwd", "密码不能为空！");
		valid=false;}
		return valid;
	}

}
