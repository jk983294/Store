package test;

import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.List;
import java.util.Random;

import org.dom4j.Document;
import org.dom4j.Element;

import support.TaskState;
import util.XmlUtil;
import business.convertor.TaskXmlConvertor;
import business.entityBeans.TaskBean;

public class XmlDocTest {
	
	public void test() {
		String file = generateTaskListXml();
		System.out.print(file);
		List<TaskBean> list = new TaskXmlConvertor().toTaskList(XmlUtil
				.buildDocFromFile(file));
		System.out.print(listToXml(list));
	}

	public String generateTaskListXml() {
		List<TaskBean> list = new ArrayList<TaskBean>();
		for (int i = 0; i < 3; i++)
			list.add(getTestTask());
		return listToXml(list);
	}

	public String listToXml(List<TaskBean> list) {
		String fileName = "D:/TaskXml/TaskList" + new Random().nextInt(100)
				+ ".xml";
		Document doc = XmlUtil.buildDocument();
		Element root = doc.addElement("tasklist");
		for (TaskBean tb : list) {
			Element task = root.addElement("task");
			task.addAttribute("id", tb.getTid());
			task.addAttribute("name", tb.getSname());
			task.addAttribute("deadLineTime", tb.getDeadlineTime().toString() );
			task.addAttribute("bidDeadLine",tb.getBidDeadline().toString());
			task.addAttribute("url", tb.getUrl());
		}
		XmlUtil.writeXML(doc, fileName, "UTF-8");
		return fileName;
	}

	/**
	 * �������һ�������������õ�TASK
	 * 
	 * @return
	 */
	private TaskBean getTestTask() {
		TaskBean tb = new TaskBean();
		// WF012012A012T123.task.xml
		// �������һ������λΪ100~999��TID
		String randomNum = String.valueOf(new Random().nextInt(900) + 100);
		tb.setTid("WF000000A000T" + randomNum);
		if (new Random().nextBoolean()) {
			tb.setSname("CPU����");
			tb.setUrl("D:/TaskXml/testOnly1.task.xml");
		} else {
			tb.setSname("�ڴ�����");
			tb.setUrl("D:/TaskXml/testOnly2.task.xml");
		}// ����ʱ�� ��ǰʱ��
		Calendar cal = Calendar.getInstance();
		Timestamp now = new Timestamp(cal.getTime().getTime());
		tb.setPubTime(now);
		// bid��ֹʱ�� 1Сʱ��
		cal.add(Calendar.HOUR_OF_DAY, 2);
		Timestamp bidDeadline = new Timestamp(cal.getTime().getTime());
		tb.setBidDeadline(bidDeadline);
		// ��� ��ֹʱ�� 1���º�
		cal.add(Calendar.MONTH, 1);
		Timestamp deadline = new Timestamp(cal.getTime().getTime());
		tb.setDeadlineTime(deadline);
		// ״̬
		tb.setState(TaskState.INITIATE);
		return tb;
	}
}
