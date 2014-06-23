package business.entityBeans;

import support.message.MessageType;

public class MessageBean {
	private String mid;
	private MessageType type;
	private String eid;
	private String content;
	private boolean viewed; // In DB it's "viewed"

	public MessageBean() {

	}

	public MessageBean(String eid, MessageType type, String content) {
		this.setEid(eid);
		this.setType(type);
		this.setContent(content);
		setViewed(false);
	}
	public String toString(){
		return this.content;
	}
	public MessageType getType() {
		return type;
	}

	public void setType(String type) {
		this.type = MessageType.valueOf(type);
	}

	public void setType(MessageType type) {
		this.type = type;
	}

	public String getEid() {
		return eid;
	}

	public void setEid(String eid) {
		this.eid = eid;
	}

	public String getContent() {
		return content;
	}

	public void setContent(String content) {
		this.content = content;
	}

	public void setViewed(boolean viewed) {
		this.viewed = viewed;
	}

	public boolean isViewed() {
		return viewed;
	}

	public void setMid(String mid) {
		this.mid = mid;
	}

	public String getMid() {
		return mid;
	}

}
