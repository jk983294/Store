package action;

import support.ThreadPoolManager;
import backStage.worker.SendOrder;

import com.opensymphony.xwork2.ActionSupport;

public class SubmitOrderAction extends ActionSupport {
	
	private static final long serialVersionUID = 11105999590579731L;
	private String orderXmlName;
	
	public String getOrderXmlName() {
		return orderXmlName;
	}

	public void setOrderXmlName(String orderXmlName) {
		this.orderXmlName = orderXmlName;
	}

	public String execute(){
		//µ÷ÓÃ·¢ËÍ
		ThreadPoolManager.execute(new SendOrder(orderXmlName));
		return "success";
	}
}
