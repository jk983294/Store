package action;

import business.entityBeans.MessageBean;
import business.managers.MessageManager;

public class InstantMessageAction extends BaseAction {

	private static final long serialVersionUID = -3625374886106607091L;
	 
	/**
	 * ��Session�л�ȡ����������EID����ѯMessage�����Ƿ��и���ҵ������Ϣ,������ʾ
	 * @return
	 */
	public String getMessage() {
		//String msg = "֤��AJAX�ڲ�ͣ�����������"+new Random().nextInt(100);
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
			msg = "��������Ϣ! �����񵽴�:[" + newTask + "],����ɹ���[" + winBidding
					+ "]������:[" + other + "]";
		}
		return msg;
	}
}
