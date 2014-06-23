package business.entityBeans;

import java.sql.Timestamp;

public class TaskEntpBean {
	private String tid;
	private String eid;
	private Timestamp bidTime;
	
	public TaskEntpBean(){
		
	}
	public TaskEntpBean(String tid,String eid,Timestamp bidTime){
		setTid(tid);setEid(eid);setBidTime(bidTime);
	}
	
	
	public String getTid() {
		return tid;
	}

	public void setTid(String tid) {
		this.tid = tid;
	}

	public String getEid() {
		return eid;
	}

	public void setEid(String eid) {
		this.eid = eid;
	}

	public Timestamp getBidTime() {
		return bidTime;
	}

	public void setBidTime(Timestamp bidTime) {
		this.bidTime = bidTime;
	}

}
