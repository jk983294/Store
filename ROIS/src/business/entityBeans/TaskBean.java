package business.entityBeans;

import java.sql.Timestamp;

import support.TaskState;

public class TaskBean {
	private String tid;
	private String sid;
	private String sname;//need to joint the service table to get
	private String eid;
	private Timestamp pubTime;
	private Timestamp startTime;
	private Timestamp endTime;
	private Timestamp deadlineTime;
	private Timestamp bidDeadline;
	private TaskState state;
	private String url; // the path of ParamXML file

	public String getTid() {
		return tid;
	}

	public void setTid(String tid) {
		this.tid = tid;
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

	public Timestamp getPubTime() {
		return pubTime;
	}

	public void setPubTime(Timestamp pubTime) {
		this.pubTime = pubTime;
	}

	public Timestamp getStartTime() {
		return startTime;
	}

	public void setStartTime(Timestamp startTime) {
		this.startTime = startTime;
	}

	public Timestamp getEndTime() {
		return endTime;
	}

	public void setEndTime(Timestamp endTime) {
		this.endTime = endTime;
	}

	public Timestamp getDeadlineTime() {
		return deadlineTime;
	}
	public void setDeadlineTime(String deadlineTime){
		try{
			this.deadlineTime=Timestamp.valueOf(deadlineTime);
		}catch(java.lang.IllegalArgumentException  ex){
			System.err.println("Error in setDeadlineTime:"+ex.getMessage());
		}
	}
	public void setDeadlineTime(Timestamp deadlineTime) {
		this.deadlineTime = deadlineTime;
	}

	public Timestamp getBidDeadline() {
		return bidDeadline;
	}
	public void setBidDeadline(String bidDeadline){
		try{
			this.bidDeadline=Timestamp.valueOf(bidDeadline);
		}catch(java.lang.IllegalArgumentException  ex){
			System.err.println("Error in setBidDeadline:"+ex.getMessage());
		}
	}
	public void setBidDeadline(Timestamp bidDeadline) {
		this.bidDeadline = bidDeadline;
	}

	public TaskState getState() {
		return state;
	}

	public void setState(TaskState state) {
		this.state = state;
	}

	public void setState(String state) {
		this.state = TaskState.valueOf(state);
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
