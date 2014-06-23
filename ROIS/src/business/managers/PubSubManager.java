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
 * 发布订阅模块的管理类
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
				// 添加任务项到TTask表
				tm.addTask(tb);
				// 查询订阅该任务的企业,通知新任务到达
				noticeEntpNewTask(sm.queryEntpBySid(tb.getSid()));
			} catch (SQLException e) {
				System.err.println(e.getMessage());
			}
		}

	}
	/**
	 * 通知相关企业有新任务发布
	 * @param list
	 */
	public void noticeEntpNewTask(List<EnterpriseBean> list) {
		for (EnterpriseBean eb : list) {
			noticeEntp(eb.getEid(), MessageType.NEW_TASK, "新任务发布!");
		}
	}
	public boolean modifySubService(String eid,SrvEntpBean myservcice){
		String url=myservcice.getUrl();
		// 1.修改XML文档。
		return new ServiceXmlConvertor().toXml(ServletActionContext.getRequest()
				.getParameterMap(), XmlUtil.buildDocFromFile(url), url);

	}
	/**
	 * 注销（退订）服务:1.删除DB记录. 2.删除文件
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
	 * 注册（订阅）服务
	 * 
	 * @param eid
	 *            企业ID
	 * @param service
	 *            服务
	 * @param url
	 *            注册服务生成的相关文档的位置
	 * @return false 说明已经注册过了
	 * @throws SQLException
	 */
	public boolean subService(String eid, ServiceBean service, String url)
			throws SQLException {
		ServiceManager sm = new ServiceManager();
		SrvEntpBean seb = new SrvEntpBean(service.getSid(), eid, url);
		// 0.检查一下是否已经处注册过了
		if (sm.querySrvcByEid(eid).contains(service)) {
			return false;
		}
		// 1.生成XML文档。
		new ServiceXmlConvertor().toXml(ServletActionContext.getRequest()
				.getParameterMap(), XmlUtil.buildDocFromFile(service.getUrl()), url);

		// 2.写入数据库
		sm.registerService(seb);
		return true;
	}
	/**
	 * 通知企业
	 * @param eid
	 * @param type
	 * @param content
	 */
	public void noticeEntp(String eid, MessageType type, String content) {
		MessageBean msg = new MessageBean(eid, type, content);
		MessageManager mm = new MessageManager();
		try {
			// 添加进数据库
			mm.addMessage(msg);
			// 添加进消息队列
			mm.getMsgQueue().setMessage(msg);
		} catch (SQLException e) {
			System.err.println(e.getMessage());
		}
	}
	
	
}
