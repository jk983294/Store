package support.message;

import business.entityBeans.MessageBean;
/**
 * 消息队列接口
 * @author Jersey
 * @date 2011-4-19
 */
public interface IMsgQueue {
	public boolean hasMsg(String eid);

	public void setMessage(MessageBean message);
	}
