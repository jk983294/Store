package action;

import java.sql.SQLException;
import java.util.List;

import support.StrutsTreeNode;
import support.StrutsTreeNodeManager;
import business.entityBeans.SrvEntpBean;
import business.managers.ServiceManager;

public class DisplayServiceRegisterAction extends BaseAction{

	private static final long serialVersionUID = 1276635607322779403L;
	private StrutsTreeNode root;
	private List<SrvEntpBean> myServices;
	public void setRoot(StrutsTreeNode root) {
		this.root = root;
	}
	public StrutsTreeNode getRoot() {
		if(root==null)
			root=StrutsTreeNodeManager.getById("1");
		return root;
	}
	public String execute(){
		root=StrutsTreeNodeManager.getById("1");
		String eid=(String) this.getSession().get("eid");
		try{
			myServices=new ServiceManager().querySrvEntpByEid(eid);
		}catch(SQLException e){
			this.addActionError("读取企业已注册服务失败");
			return INPUT;
		}
		return SUCCESS;
	}
	public void setMyServices(List<SrvEntpBean> myServices) {
		this.myServices = myServices;
	}
	public List<SrvEntpBean> getMyServices() {
		return myServices;
	}
	
}
