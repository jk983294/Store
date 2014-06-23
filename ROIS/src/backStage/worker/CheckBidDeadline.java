package backStage.worker;

import java.sql.SQLException;
import java.util.List;
import java.util.TimerTask;

import javax.servlet.ServletContext;

import org.dom4j.Document;
import org.dom4j.Element;

import support.GlobalParameter;
import support.TaskState;
import support.ThreadPoolManager;
import util.TimeUtil;
import util.XmlUtil;
import business.entityBeans.TaskBean;
import business.entityBeans.TaskEntpBean;
import business.managers.TaskManager;

/**
 * 后台线程，检查DB中是否有竞标截止时间到期的任务，<br/>
 * 如果有则调用{@link SendBidders}线程发送竞标企业列表
 * @see SendBidders
 * @author Jersey
 * @date 2011-4-19
 */
public class CheckBidDeadline extends TimerTask {
	private String biddingXml;
	private ServletContext context;

	public CheckBidDeadline(ServletContext context) {
		this.context = context;
	}

	@Override
	public void run() {
		context.log("开始检查Rois.TTask表中是否有bidDeadline到期的task");
		TaskManager tm = new TaskManager();
		try {
			List<TaskBean> tasks = tm.checkBidDeadline();
			if (null != tasks && tasks.size() > 0) {
				// 有任务项的竞标截止日期到了
				biddingXml = GlobalParameter.BiddingXmlDir() + "biddingXml_"
						+ TimeUtil.getNowString() + ".xml";// OPTIMIZE_ME：从GlobalParameter中读取
				Document doc = XmlUtil.buildDocument();
				Element root = doc.addElement("tasklist");
				for (TaskBean tb : tasks) {
					Element taskTag = root.addElement("task").addAttribute(
							"id", tb.getTid());
					// 1.该去Map_Task_Entp表中查得EID.
					List<TaskEntpBean> list = tm
							.queryTaskEntpByTid(tb.getTid());
					for (TaskEntpBean teb : list) {
						taskTag.addElement("bidder").addAttribute("id",
								teb.getEid());
					}
					// 2.更新这些Task 状态为Bidding
					tm.updateTaskState(tb.getTid(), TaskState.BIDDING);
				}
				// 3.生成竞标企业列表XML
				if (XmlUtil.writeXML(doc, biddingXml, "UTF-8")) {

					// 4.调用SendOrder线程发送。
					ThreadPoolManager.execute(new SendBidders(biddingXml));
				} else {
					System.err.println(biddingXml + "文档生成失败!");
				}
			}
		} catch (SQLException e) {
			System.err.println(e.getMessage());
		}
	}
}
