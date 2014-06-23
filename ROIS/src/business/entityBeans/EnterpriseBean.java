package business.entityBeans;

public class EnterpriseBean {
	private String ename;
	private String loginName;
	private String loginPwd;
	private String location;
	private String email;
	private String phone;
	private String desc;
	private String eid;

	public String getEname() {
		return ename;
	}

	public void setEname(String ename) {
		this.ename = ename;
	}

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

	public String getLocation() {
		return location;
	}

	public void setLocation(String location) {
		this.location = location;
	}

	public String getEmail() {
		return email;
	}

	public void setEmail(String email) {
		this.email = email;
	}

	public String getPhone() {
		return phone;
	}

	public void setPhone(String phone) {
		this.phone = phone;
	}

	public String getDesc() {
		return desc;
	}

	public void setDesc(String desc) {
		this.desc = desc;
	}

	public String getEid() {
		return eid;
	}

	public void setEid(String eid) {
		this.eid = eid;
	}

	public String toString() {
		return "Eid: [" + getEid() + "]\n Ename: [" + getEname()
				+ "]\n LoginName: [" + getLoginName() + "]\n LoginPassword: ["
				+ getLoginPwd() + "]\n Location: [" + getLocation()
				+ "]\n Email: [" + getEmail() + "]\n Phone: [" + getPhone()
				+ "]\n Description: [" + getDesc() + "]\n";

	}
}
