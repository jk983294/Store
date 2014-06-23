package action;

import java.util.ArrayList;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpSession;
import javax.servlet.jsp.PageContext;

import org.apache.struts2.ServletActionContext;

import business.convertor.OrderXmlConvertor;
import business.entityBeans.OrderFormBean;
import business.entityBeans.OrderResultBean;

import com.opensymphony.xwork2.ActionSupport;

//接收订单用户提交的订单
public class FillOrderAction extends ActionSupport {
	private String ordernumber;
	private String orderDetailHtml;
	
	private String processName;//定义名称
	private OrderResultBean resultBean;
	
	public void setResultBean(OrderResultBean bean) {
		this.resultBean = bean;
	}

	public OrderResultBean getResultBean() {
		return resultBean;
	}
	
	public String getOrderDetailHtml(){
		return orderDetailHtml;
	}
	
	public String execute() {
		// 获取参数的所有名称
		HttpServletRequest request = ServletActionContext.getRequest();
		String ordername = (String) request.getSession().getAttribute(
				"ordername");
		
		resultBean = new OrderResultBean();
		resultBean.setHttpRequest(request);
		resultBean.setOrderName(ordername);
		orderDetailHtml = resultBean.getResultHtml();
		// 生成订单xml文件_dom
		resultBean.writeOrderXml();
		System.out.println("生成Xml文件："+resultBean.getXmlFile());

		return "success";
	}
}
