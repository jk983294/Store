package action;

import java.sql.SQLException;
import java.util.List;

import support.StrutsTreeNode;
import support.StrutsTreeNodeManager;
import business.convertor.ServiceXmlConvertor;
import business.entityBeans.ServiceBean;
import business.entityBeans.SrvEntpBean;
import business.managers.ServiceManager;

public class DisplayServiceDetailAction extends BaseAction {
	private static final long serialVersionUID = 7178257950758349964L;
	private String nodeId;
	private StrutsTreeNode currentNode;
	private ServiceBean service;
	private SrvEntpBean myService;
	private String sid;
	private String html;

	public String getSid() {
		return sid;
	}

	public void setSid(String sid) {
		this.sid = sid;
	}

	public String getHtml() {
		return html;
	}

	public void setNodeId(String nodeId) {
		this.nodeId = nodeId;
	}

	public static List<ServiceBean> serviceList;

	public String getNodeId() {
		return nodeId;
	}

	public String serviceDetail() {
		currentNode = StrutsTreeNodeManager.getById(nodeId);
		ServiceManager sm = new ServiceManager();
		// 找到对应Service
		try {
			service = sm.queryByName(currentNode.getName());
		} catch (SQLException e) {
			return NONE;
		}
		if (service != null)
			html = new ServiceXmlConvertor().toHtml(service.getUrl());
		return SUCCESS;
	}

	public String myserviceDetail() {
		if (sid != null){
			String eid=(String)getSession().get("eid");
			try {
				myService=new ServiceManager().querySrvEntpByEidSid(eid, sid);
			} catch (SQLException e) {
				this.addActionError("读取企业已注册服务失败");
				return INPUT;
			}
			html = new ServiceXmlConvertor().toHtml(myService.getUrl());
		}
		return SUCCESS;
	}

	public String getNodeName() {
		return currentNode != null ? currentNode.getName() : "Node not found";
	}

	public void setService(ServiceBean sb) {
		this.service = sb;
	}

	public ServiceBean getService() {
		return service;
	}

	public void setMyService(SrvEntpBean myService) {
		this.myService = myService;
	}

	public SrvEntpBean getMyService() {
		return myService;
	}

}
