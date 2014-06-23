package business.entityBeans;

public class ServiceBean {
	private String sid;
	private String sname;
	private String url;
	private float qos_value;
	@Override
	public boolean equals(Object o) {
		if (!(o instanceof ServiceBean))
			return false;
		ServiceBean s = (ServiceBean) o;
		return this.sid.equals(s.sid) && this.sname.equals(s.sname)
				&& this.url.equals(s.url);
	}
	public ServiceBean(){
		
	}
	public ServiceBean(String sname,String url){
		this.sname=sname;
		this.url=url;
	}
	public String getSid() {
		return sid;
	}

	public void setSid(String sid) {
		this.sid = sid;
	}

	public String getSname() {
		return sname;
	}

	public void setSname(String sname) {
		this.sname = sname;
	}

	public String getUrl() {
		return url;
	}

	public void setUrl(String url) {
		this.url = url;
	}
	
	public float getQos_value() {
		return qos_value;
	}
	
	public void setQos_value(float qos_value) {
		this.qos_value = qos_value;
	}

}
