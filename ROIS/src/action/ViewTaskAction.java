package action;

import java.net.URL;
import java.sql.SQLException;
import java.util.List;

import org.dom4j.Document;

import util.XmlUtil;
import business.convertor.TaskXmlConvertor;
import business.entityBeans.TaskBean;
import business.managers.TaskManager;

public class ViewTaskAction extends BaseAction {

	private static final long serialVersionUID = 6255564620132820894L;
	private List<TaskBean> taskList;
	private List<TaskBean> myTaskList;
	private String xmlUrl;
	private String detailXml;
	
	/**
	 * 1.获取订阅的任务项列表. 2.获取企业自己的任务项列表
	 * @return
	 */
	public String taskTable() {
		TaskManager tm = new TaskManager();
		String eid = (String) getSession().get("eid");
		try {
			taskList = tm.querySubTaskByEid(eid);
			myTaskList=tm.queryMyTaskByEid(eid);
		} catch (SQLException e) {
			System.err.println(e.getMessage());
			return ERROR;
		}
		return "taskTable";
	}

	public String detail() {
		Document doc = XmlUtil.buildDocFromURL(xmlUrl);
		setDetailXml(new TaskXmlConvertor().toHtml(doc));
		return "detail";
	}

	public void setTaskList(List<TaskBean> taskList) {
		this.taskList = taskList;
	}

	public List<TaskBean> getTaskList() {
		return taskList;
	}

	public void setXmlUrl(String xmlUrl) {
		this.xmlUrl = xmlUrl;
	}

	public String getXmlUrl() {
		return xmlUrl;
	}

	public void setDetailXml(String detailXml) {
		this.detailXml = detailXml;
	}

	public String getDetailXml() {
		return detailXml;
	}

	public void setMyTaskList(List<TaskBean> myTaskList) {
		this.myTaskList = myTaskList;
	}

	public List<TaskBean> getMyTaskList() {
		return myTaskList;
	}
	
	public String getTaskListMsg(){
		return this.getMessage("taskListMsg");
	}

	public String getMyTaskListMsg() {
		return this.getMessage("myTaskListMsg");
	}
}
