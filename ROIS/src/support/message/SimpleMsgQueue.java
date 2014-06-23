package support.message;

import java.util.Vector;

import business.entityBeans.MessageBean;

/**
 * Not in use yet
 * 
 * @author Jersey
 * @UNUSED
 */
public class SimpleMsgQueue implements IMsgQueue {
	private Vector<String> queue;

	@Override
	public boolean hasMsg(String eid) {
		return queue.contains(eid);

	}

	public synchronized boolean removeMessage(String eid) {

		return queue.remove(eid);
	}

	@Override
	public synchronized void setMessage(MessageBean message) {
		if (!this.hasMsg(message.getEid()))
			queue.add(message.getEid());
	}

}
