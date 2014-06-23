package backStage.worker;

import java.sql.SQLException;
import java.util.Calendar;
import java.util.List;

import support.TaskState;
import util.TimeUtil;
import util.XmlUtil;
import business.convertor.TaskXmlConvertor;
import business.entityBeans.ServiceBean;
import business.entityBeans.TaskBean;
import business.managers.PubSubManager;
import business.managers.ServiceManager;

/**
 * 后台线程，完成接收到任务列表XML文档之后的发布工作
 * 
 * @author Jersey
 * 
 */
public class RecvTaskList implements Runnable {

	private String xmlPath;

	public RecvTaskList(String path) {
		xmlPath = path;
	}

	@Override
	public void run() {
		System.out.println("后台接收任务列表线程被调用");
		PubSubManager ps = new PubSubManager();
		ps.pubTasks(this.parseXml());
	}

	public List<TaskBean> parseXml() {
		List<TaskBean> list = new TaskXmlConvertor().toTaskList(XmlUtil
				.buildDocFromFile(xmlPath));
		/*
		 * 上面的list是从XML文档读出来的，还需要进行TaskBean的信息补全 主要是pubTime,sid两项和state OPTIMIZE
		 * ME:每个都查询DB，性能不好！
		 */
		for (TaskBean tb : list) {
			tb.setPubTime(TimeUtil.getNowTimestamp());
			tb.setState(TaskState.INITIATE);
			ServiceManager sm = new ServiceManager();
			try {
				ServiceBean s = sm.queryByName(tb.getSname());
				tb.setSid(s.getSid());
				// TODO:为演示方便，在ROIS端给BIDDEADLINE加1
				tb.setBidDeadline(TimeUtil
						.getTimestamp(Calendar.DAY_OF_MONTH, 1));
			} catch (SQLException e) {
				System.err.println(e.getMessage());
			}
		}

		return list;
	}

}
