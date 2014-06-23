package support.message;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import business.entityBeans.MessageBean;

/**
 * 消息队列的MAP实现，KEY为eid，Value为List<MessageBean>>
 * @OPTIMIZE_ME：should be more simple
 * @author Jersey
 * 
 */
public class MsgQueueMapImpt implements IMsgQueue{
	private Map<String, List<MessageBean>> msgQueue;

	public MsgQueueMapImpt() {
		msgQueue = new HashMap<String, List<MessageBean>>();
	}

	public boolean hasMsg(String eid) {
		return msgQueue.containsKey(eid);
	}

	// public Map<String, List<MessageBean>> getMessageQueue() {
	// if (msgQueue == null)
	// msgQueue = new HashMap<String, List<MessageBean>>();
	// return msgQueue;
	// }

	public synchronized List<MessageBean> getMessage(String eid) {
		return msgQueue.get(eid);
	}
	public synchronized List<MessageBean> removeMessage(String eid){
		return msgQueue.remove(eid);
	}
	public synchronized void setMessage(MessageBean message) {
		String eid = message.getEid();
		if (msgQueue.containsKey(eid)) {
			msgQueue.get(eid).add(message);
		} else {
			List<MessageBean> list = new ArrayList<MessageBean>();
			list.add(message);
			msgQueue.put(eid, list);
		}
	}
}
