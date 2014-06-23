package business.managers;

import java.io.File;
import java.sql.SQLException;
import java.util.List;

import org.apache.struts2.ServletActionContext;

import business.convertor.ServiceXmlConvertor;
import business.entityBeans.EnterpriseBean;
import business.entityBeans.MessageBean;
import business.entityBeans.ServiceBean;
import business.entityBeans.SrvEntpBean;
import business.entityBeans.TaskBean;

import support.message.MessageType;
import util.XmlUtil;
/**
 * ��������ģ��Ĺ�����
 * @author Jersey
 * @date 2011-4-19
 */
public class PubSubManager {

	/**
	 * Add new tasks into Db.TTask. Find related eid with sid in each task.
	 * Notify eusers
	 * 
	 * @param tbs
	 */
	public void pubTasks(List<TaskBean> tbs) {
		TaskManager tm = new TaskManager();
		ServiceManager sm = new ServiceManager();
		// String sid=null,eid=null;
		for (TaskBean tb : tbs) {
			try {
				// ��������TTask��
				tm.addTask(tb);
				// ��ѯ���ĸ��������ҵ,֪ͨ�����񵽴�
				noticeEntpNewTask(sm.queryEntpBySid(tb.getSid()));
			} catch (SQLException e) {
				System.err.println(e.getMessage());
			}
		}

	}
	/**
	 * ֪ͨ�����ҵ�������񷢲�
	 * @param list
	 */
	public void noticeEntpNewTask(List<EnterpriseBean> list) {
		for (EnterpriseBean eb : list) {
			noticeEntp(eb.getEid(), MessageType.NEW_TASK, "�����񷢲�!");
		}
	}
	public boolean modifySubService(String eid,SrvEntpBean myservcice){
		String url=myservcice.getUrl();
		// 1.�޸�XML�ĵ���
		return new ServiceXmlConvertor().toXml(ServletActionContext.getRequest()
				.getParameterMap(), XmlUtil.buildDocFromFile(url), url);

	}
	/**
	 * ע�����˶�������:1.ɾ��DB��¼. 2.ɾ���ļ�
	 * @param seb
	 * @return
	 */
	public boolean desubService(SrvEntpBean seb){
		ServiceManager sm = new ServiceManager();
		try {
			sm.deleteSrvEntp(seb);
			return XmlUtil.delete(new File(seb.getUrl()));
		} catch (SQLException e) {
			System.err.println("desubService error:"+e.getMessage());
			return false;
		}
		
	}
	/**
	 * ע�ᣨ���ģ�����
	 * 
	 * @param eid
	 *            ��ҵID
	 * @param service
	 *            ����
	 * @param url
	 *            ע��������ɵ�����ĵ���λ��
	 * @return false ˵���Ѿ�ע�����
	 * @throws SQLException
	 */
	public boolean subService(String eid, ServiceBean service, String url)
			throws SQLException {
		ServiceManager sm = new ServiceManager();
		SrvEntpBean seb = new SrvEntpBean(service.getSid(), eid, url);
		// 0.���һ���Ƿ��Ѿ���ע�����
		if (sm.querySrvcByEid(eid).contains(service)) {
			return false;
		}
		// 1.����XML�ĵ���
		new ServiceXmlConvertor().toXml(ServletActionContext.getRequest()
				.getParameterMap(), XmlUtil.buildDocFromFile(service.getUrl()), url);

		// 2.д�����ݿ�
		sm.registerService(seb);
		return true;
	}
	/**
	 * ֪ͨ��ҵ
	 * @param eid
	 * @param type
	 * @param content
	 */
	public void noticeEntp(String eid, MessageType type, String content) {
		MessageBean msg = new MessageBean(eid, type, content);
		MessageManager mm = new MessageManager();
		try {
			// ��ӽ����ݿ�
			mm.addMessage(msg);
			// ��ӽ���Ϣ����
			mm.getMsgQueue().setMessage(msg);
		} catch (SQLException e) {
			System.err.println(e.getMessage());
		}
	}
	
	
}
