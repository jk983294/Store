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
 * 后台线程:执行接收中标企业名单后的操作
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
	 * 0.解析收到的List<br/>
	 * 1.修改Ttask，更新EID和STATE。<br/>
	 * 2.从MAP_TASK_ENTP中删除相关记录where tid=tid。<br/>
	 * 3.通知E.竞标成功
	 */
	@Override
	public void run() {
		System.out.println("后台线程 RecvBidWinner开始执行");
		// 0.解析收到的List
		this.winners = this.parseBidWinnerFile(xmlPath);
		// 1.修改Ttask，更新EID和STATE。 2.从MAP_TASK_ENTP中删除相关记录where
		// tid=tid。3.通知E.竞标成功
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
			ps.noticeEntp(eid, MessageType.WIN_BIDDING, "恭喜您！任务[" + tid
					+ "]竞标成功");
		}
		System.out.println("后台线程 RecvBidWinner执行完毕");
	}

	public Map<String, String> parseBidWinnerFile(String path) {
		Map<String, String> winners = new HashMap<String, String>();
		Document doc = XmlUtil.buildDocFromFile(path);
		Element root = doc.getRootElement();
		if (!root.getName().toLowerCase().equals("tasklist")) {
			System.err.println("任务列表XML文档格式错误！");
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
