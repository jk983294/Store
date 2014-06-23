package action;

import business.entityBeans.MessageBean;
import business.managers.MessageManager;

public class InstantMessageAction extends BaseAction {

	private static final long serialVersionUID = -3625374886106607091L;
	 
	/**
	 * 从Session中获取发起该请求的EID，查询Message队列是否有该企业的新消息,有则显示
	 * @return
	 */
	public String getMessage() {
		//String msg = "证明AJAX在不停请求服务器："+new Random().nextInt(100);
		String msg="";
		String eid = this.getSession().get("eid").toString();
		MessageManager mm = new MessageManager();
		if (mm.getMsgQueue().hasMsg(eid)) {
			int newTask = 0, winBidding = 0, other = 0;
			for (MessageBean mb : mm.getMsgQueue().getMessage(eid)) {
				switch (mb.getType()) {
				case NEW_TASK:
					newTask++;
					break;
				case WIN_BIDDING:
					winBidding++;
					break;
				default:
					other++;
					break;
				}
			}
			msg = "您有新消息! 新任务到达:[" + newTask + "],竞标成功：[" + winBidding
					+ "]，其他:[" + other + "]";
		}
		return msg;
	}
}
