package business.entityBeans;

public class SrvEntpBean {
	private String sid;
	private String sname;
	private String eid;
	private String url;
	public SrvEntpBean(){
		
	}
	public SrvEntpBean(String sid, String eid, String url) {
		setSid(sid);
		setEid(eid);
		setUrl(url);
	}

	@Override
	public boolean equals(Object o) {
		if (!(o instanceof SrvEntpBean))
			return false;
		SrvEntpBean s = (SrvEntpBean) o;
		return this.sid.equals(s.sid) && this.eid.equals(s.eid)
				&& this.url.equals(s.url);
	}

	public String getSid() {
		return sid;
	}

	public void setSid(String sid) {
		this.sid = sid;
	}

	public String getEid() {
		return eid;
	}

	public void setEid(String eid) {
		this.eid = eid;
	}

	public String getUrl() {
		return url;
	}

	public void setUrl(String url) {
		this.url = url;
	}
	public void setSname(String sname) {
		this.sname = sname;
	}
	public String getSname() {
		return sname;
	}
	

}
