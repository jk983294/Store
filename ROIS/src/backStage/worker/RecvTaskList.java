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
 * ��̨�̣߳���ɽ��յ������б�XML�ĵ�֮��ķ�������
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
		System.out.println("��̨���������б��̱߳�����");
		PubSubManager ps = new PubSubManager();
		ps.pubTasks(this.parseXml());
	}

	public List<TaskBean> parseXml() {
		List<TaskBean> list = new TaskXmlConvertor().toTaskList(XmlUtil
				.buildDocFromFile(xmlPath));
		/*
		 * �����list�Ǵ�XML�ĵ��������ģ�����Ҫ����TaskBean����Ϣ��ȫ ��Ҫ��pubTime,sid�����state OPTIMIZE
		 * ME:ÿ������ѯDB�����ܲ��ã�
		 */
		for (TaskBean tb : list) {
			tb.setPubTime(TimeUtil.getNowTimestamp());
			tb.setState(TaskState.INITIATE);
			ServiceManager sm = new ServiceManager();
			try {
				ServiceBean s = sm.queryByName(tb.getSname());
				tb.setSid(s.getSid());
				// TODO:Ϊ��ʾ���㣬��ROIS�˸�BIDDEADLINE��1
				tb.setBidDeadline(TimeUtil
						.getTimestamp(Calendar.DAY_OF_MONTH, 1));
			} catch (SQLException e) {
				System.err.println(e.getMessage());
			}
		}

		return list;
	}

}
