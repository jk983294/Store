package action;

import org.apache.struts2.ServletActionContext;

import com.opensymphony.xwork2.ActionSupport;

public class SelectOrderAction extends ActionSupport {	

	private static final long serialVersionUID = 5329272642564966631L;

	//根据用户选择的订单，生成订单页面，如计算机页面，则显示CPU和显卡的制造参数
	public String execute(){
		//获得全局的request，另外一种方法是IoC方法（未了解）
		String ordername = ServletActionContext.getRequest().getParameter("ordername");
		//设置跳转到下个jsp页面OrderPage.jsp的request
		ServletActionContext.getRequest().getSession().setAttribute("ordername", ordername);
		return "success";
	}
	
}
