package action;

import java.sql.SQLException;
import java.util.List;

import business.entityBeans.MessageBean;
import business.managers.MessageManager;

public class MessageBoxAction extends BaseAction {

	private static final long serialVersionUID = -723677788179467311L;
	private List<MessageBean> msgList;

	public void setMsgList(List<MessageBean> msgList) {
		this.msgList = msgList;
	}

	public List<MessageBean> getMsgList() {
		return msgList;
	}

	public String execute() {
		// 获取EID，从DB读取E的消息
		String eid = this.getSession().get("eid").toString();
		MessageManager mm = new MessageManager();
		try {
			mm.getMsgQueue().removeMessage(eid);//从消息队列中去掉EID
			msgList = mm.queryByEid(eid);
			return SUCCESS;
		} catch (SQLException e) {
			return ERROR;
		}
	}
}
