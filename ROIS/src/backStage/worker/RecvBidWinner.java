package backStage.worker;

import java.sql.SQLException;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

import org.dom4j.Document;
import org.dom4j.Element;

import support.message.MessageType;
import util.XmlUtil;
import business.managers.PubSubManager;
import business.managers.TaskManager;

/**
 * ��̨�߳�:ִ�н����б���ҵ������Ĳ���
 * 
 * @author Jersey
 * @date 2011-4-19
 */
public class RecvBidWinner implements Runnable {
	// Key Stands for tid, value stands for eid
	private Map<String, String> winners;
	private String xmlPath;

	public RecvBidWinner(String xmlPath) {
		this.xmlPath = xmlPath;
	}

	/**
	 * 0.�����յ���List<br/>
	 * 1.�޸�Ttask������EID��STATE��<br/>
	 * 2.��MAP_TASK_ENTP��ɾ����ؼ�¼where tid=tid��<br/>
	 * 3.֪ͨE.����ɹ�
	 */
	@Override
	public void run() {
		System.out.println("��̨�߳� RecvBidWinner��ʼִ��");
		// 0.�����յ���List
		this.winners = this.parseBidWinnerFile(xmlPath);
		// 1.�޸�Ttask������EID��STATE�� 2.��MAP_TASK_ENTP��ɾ����ؼ�¼where
		// tid=tid��3.֪ͨE.����ɹ�
		TaskManager tm = new TaskManager();
		PubSubManager ps = new PubSubManager();
		for (String tid : winners.keySet()) {
			String eid = winners.get(tid);
			try {
				tm.updateTaskOnwer(tid, eid);
				tm.DeleteBidding(tid);
			} catch (SQLException e) {
				System.err.println(e.getMessage());
			}
			ps.noticeEntp(eid, MessageType.WIN_BIDDING, "��ϲ��������[" + tid
					+ "]����ɹ�");
		}
		System.out.println("��̨�߳� RecvBidWinnerִ�����");
	}

	public Map<String, String> parseBidWinnerFile(String path) {
		Map<String, String> winners = new HashMap<String, String>();
		Document doc = XmlUtil.buildDocFromFile(path);
		Element root = doc.getRootElement();
		if (!root.getName().toLowerCase().equals("tasklist")) {
			System.err.println("�����б�XML�ĵ���ʽ����");
			return null;
		}
		Iterator<?> it = root.elementIterator("task");
		while (it.hasNext()) {
			Element task = (Element) it.next();
			winners.put(task.attributeValue("id"),
					task.attributeValue("winner"));
		}
		return winners;
	}
}
