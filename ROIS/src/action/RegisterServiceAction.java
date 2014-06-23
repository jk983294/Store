package action;

import java.sql.SQLException;

import support.GlobalParameter;

import business.entityBeans.ServiceBean;
import business.entityBeans.SrvEntpBean;
import business.managers.PubSubManager;

public class RegisterServiceAction extends BaseAction {

	private static final long serialVersionUID = -30985446649735223L;
	private String message;
	private ServiceBean service;
	private SrvEntpBean myService;

	public void setMessage(String message) {
		this.message = message;
	}

	public String getMessage() {
		return message;
	}

	public String execute() {
		// this.setMessage(service.getSname());
		return SUCCESS;
	}

	/**
	 * 修改已经注册了的服务
	 * 
	 * @return
	 */
	public String modify() {
		String eid = (String) getSession().get("eid");
		PubSubManager ps = new PubSubManager();
		ps.modifySubService(eid, myService);
		setMessage(myService.getSname() + " 服务修改成功");
		return SUCCESS;
	}

	/**
	 * 注销服务。
	 * 
	 * @return
	 */
	public String cancel() {
		PubSubManager ps = new PubSubManager();
		if (!ps.desubService(myService)) {
			this.addActionError("注销服务" + service.getSname() + "失败！请查看日志");
			return INPUT;
		}
		setMessage(myService.getSname() + " 服务注销成功");
		return SUCCESS;
	}

	/**
	 * 注册新服务
	 * 
	 * @return
	 */
	public String register() {
		String eid = (String) getSession().get("eid");
		String outputPath = GlobalParameter.SrvEntpXmlDir() + "srvc"
				+ service.getSid() + "entp" + eid + ".xml";
		// 调用发布订阅模块订阅服务
		PubSubManager ps = new PubSubManager();
		try {
			if (!ps.subService(eid, service, outputPath)) {
				this.addActionError("您已经注册过服务" + service.getSname() + "！");
				return INPUT;
			}
		} catch (SQLException e) {
			return ERROR;
		}
		setMessage(service.getSname() + " 服务注册成功");
		return SUCCESS;

	}

	public void setService(ServiceBean service) {
		this.service = service;
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
