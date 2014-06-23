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
 * ��̨�̣߳����DB���Ƿ��о����ֹʱ�䵽�ڵ�����<br/>
 * ����������{@link SendBidders}�̷߳��;�����ҵ�б�
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
		context.log("��ʼ���Rois.TTask�����Ƿ���bidDeadline���ڵ�task");
		TaskManager tm = new TaskManager();
		try {
			List<TaskBean> tasks = tm.checkBidDeadline();
			if (null != tasks && tasks.size() > 0) {
				// ��������ľ����ֹ���ڵ���
				biddingXml = GlobalParameter.BiddingXmlDir() + "biddingXml_"
						+ TimeUtil.getNowString() + ".xml";// OPTIMIZE_ME����GlobalParameter�ж�ȡ
				Document doc = XmlUtil.buildDocument();
				Element root = doc.addElement("tasklist");
				for (TaskBean tb : tasks) {
					Element taskTag = root.addElement("task").addAttribute(
							"id", tb.getTid());
					// 1.��ȥMap_Task_Entp���в��EID.
					List<TaskEntpBean> list = tm
							.queryTaskEntpByTid(tb.getTid());
					for (TaskEntpBean teb : list) {
						taskTag.addElement("bidder").addAttribute("id",
								teb.getEid());
					}
					// 2.������ЩTask ״̬ΪBidding
					tm.updateTaskState(tb.getTid(), TaskState.BIDDING);
				}
				// 3.���ɾ�����ҵ�б�XML
				if (XmlUtil.writeXML(doc, biddingXml, "UTF-8")) {

					// 4.����SendOrder�̷߳��͡�
					ThreadPoolManager.execute(new SendBidders(biddingXml));
				} else {
					System.err.println(biddingXml + "�ĵ�����ʧ��!");
				}
			}
		} catch (SQLException e) {
			System.err.println(e.getMessage());
		}
	}
}
