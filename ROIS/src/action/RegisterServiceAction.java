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
	 * �޸��Ѿ�ע���˵ķ���
	 * 
	 * @return
	 */
	public String modify() {
		String eid = (String) getSession().get("eid");
		PubSubManager ps = new PubSubManager();
		ps.modifySubService(eid, myService);
		setMessage(myService.getSname() + " �����޸ĳɹ�");
		return SUCCESS;
	}

	/**
	 * ע������
	 * 
	 * @return
	 */
	public String cancel() {
		PubSubManager ps = new PubSubManager();
		if (!ps.desubService(myService)) {
			this.addActionError("ע������" + service.getSname() + "ʧ�ܣ���鿴��־");
			return INPUT;
		}
		setMessage(myService.getSname() + " ����ע���ɹ�");
		return SUCCESS;
	}

	/**
	 * ע���·���
	 * 
	 * @return
	 */
	public String register() {
		String eid = (String) getSession().get("eid");
		String outputPath = GlobalParameter.SrvEntpXmlDir() + "srvc"
				+ service.getSid() + "entp" + eid + ".xml";
		// ���÷�������ģ�鶩�ķ���
		PubSubManager ps = new PubSubManager();
		try {
			if (!ps.subService(eid, service, outputPath)) {
				this.addActionError("���Ѿ�ע�������" + service.getSname() + "��");
				return INPUT;
			}
		} catch (SQLException e) {
			return ERROR;
		}
		setMessage(service.getSname() + " ����ע��ɹ�");
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
