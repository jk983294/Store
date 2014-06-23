package business.entityBeans;
import business.convertor.OrderXmlConvertor;


public class OrderFormBean{
	private String processName;
	private OrderXmlConvertor convertor = new OrderXmlConvertor(); 
	
	public OrderFormBean() {
	}
	
	public void setProcessName(String processName) {
		this.processName = processName;
	}
	
	public String getOrderFormHtml() {
		convertor.setProcessName(processName);
		return convertor.toOrderFormHtml();
	}

}