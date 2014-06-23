package business.convertor;
import java.util.ArrayList;

import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import support.GlobalParameter;
import util.parser.XmlParser;

public class OrderXmlConvertor extends XmlParser{
	private String XmlBasePath;
	private String processName;
	private final String xmlExt = ".process.xml";
	
	private ArrayList<String> XmlUrls = new ArrayList<String>();
	private ArrayList<String> services = new ArrayList<String>();
	private ArrayList<String> params = new ArrayList<String>();
	
	//订单填写页面
	private String orderFormHtml;
	
	public enum ParamType {
		TEXT, CHECKBOX, RADIO, SELECT
	}
	
	public OrderXmlConvertor(){
		XmlBasePath = GlobalParameter.OrderXmlDir();
		
	}

	public void setProcessName(String processName) {
		this.processName = processName;
	}


	//获得朝早流程中包含的制造服务名称列表
	public ArrayList<String> getServiceName(String ordername){
		services.clear();
		initNewXmlParser(XmlBasePath+ processName + xmlExt);
		//System.out.println("获取订单的所有制造服务名："+ processName);
		
		NodeList tasks= doc.getElementsByTagName("task");
		int taskNum = tasks.getLength();
		for (int i= 0; i < taskNum; i++){
			if (tasks.item(i).getNodeType() == Node.ELEMENT_NODE){
				services.add(tasks.item(i).getAttributes().getNamedItem("name").getNodeValue());
			}
		}
		
		destroyXmlParser();
		return services;
	}
	
	//获取制造流程中某项制造服务的定义xml路径
	public String getServiceXmlUrl(String servicename){
		String serviceXmlUrl=GlobalParameter.ServiceUrlBiMap().get(servicename);
		return serviceXmlUrl;
	}
	
	//获得制造服务的参数名称列表
	//根据服务名称，获取服务定义xml路径，然后读取参数名称
	public ArrayList<String> getParamName(String servicename){

		String sXmlUrl = getServiceXmlUrl(servicename);
		params.clear();
		//sXmlUrl中已经包含.xml，所以不用加xmlExt
		initNewXmlParser(sXmlUrl);
		
		NodeList nodes = doc.getElementsByTagName("param");
		int nodeNum = nodes.getLength();
		for (int i = 0; i < nodeNum; i++){
			if (nodes.item(i).getNodeType() == Node.ELEMENT_NODE){
				String paramName = nodes.item(i).getAttributes().getNamedItem("name").getNodeValue();
			
				params.add(paramName);
			}
		}
		
		destroyXmlParser();
		
		return params;
	}
	
	//modified: 4.27 by zhd，分开两种text_value和min max
	public String getParamType(String serviceName, String paramName){
		String sXmlUrl = getServiceXmlUrl(serviceName);
		//sXmlUrl中已经包含.xml，所以不用加xmlExt
		initNewXmlParser(sXmlUrl);
		NodeList nodes = doc.getElementsByTagName("param");
		int nodeNum = nodes.getLength();
		String paramType = "";
		
		for (int i = 0; i < nodeNum; i++){
			if (nodes.item(i).getNodeType() == Node.ELEMENT_NODE){
				if (nodes.item(i).getAttributes().getNamedItem("name").getNodeValue().equals(paramName)){
					paramType = nodes.item(i).getAttributes().getNamedItem("type").getNodeValue();
					//text类型的参数需要特殊处理
					if(paramType.equals("text")){
						Node node = nodes.item(i).getAttributes().getNamedItem("value");
						if (node != null){
							paramType += "_value";
						}
						else
							paramType += "_min_max";
					}
				}
			}
		}
		
		
		destroyXmlParser();
		return paramType;
	}
	
	
	public String toOrderFormHtml(){
		//流程定义文件的绝对地址，目前只能这么做，取相对地址不可行
		initNewXmlParser(XmlBasePath+processName + xmlExt);

		//订单中 制造任务的制造参数xml文件
		NodeList xmlUrlNodes=doc.getElementsByTagName("task");
		XmlUrls.clear();
		if(xmlUrlNodes != null)
		{
			int len = xmlUrlNodes.getLength();
			String XmlUrl;
			
			for (int i = 0; i < len; i++){
				Node curNode=xmlUrlNodes.item(i);
				if (curNode.getNodeType() == Node.ELEMENT_NODE){
					//System.out.println(xmlUrlNodes.item(i).getNodeName());
					//去除掉空格
					//System.out.println(xmlUrlNodes.item(i).getFirstChild().getNodeValue().trim());
					String taskName=curNode.getAttributes().getNamedItem("name").getNodeValue().trim();
					XmlUrl = GlobalParameter.ServiceUrlBiMap().get(taskName);
			
					XmlUrls.add(XmlUrl);
				}
			}
		}
		destroyXmlParser();
		
		//一个表单
		orderFormHtml = "<table border = '1' style=\"width:60%;margin:0 auto;\">";
		//解析制造定义xml文件
		int XmlSize = XmlUrls.size();
		String serviceName;//制造服务名称
		for (int i = 0; i < XmlSize; i++){
			initNewXmlParser(XmlUrls.get(i));
			
			//根结点可以轻松获得属性值，制造服务名称
			serviceName = doc.getDocumentElement().getAttribute("name");
			//System.out.println(serviceName);
			
			orderFormHtml += "<tr>";
			orderFormHtml += "<td class = \"service\" width = \"\40%\">";
			orderFormHtml += serviceName;
			orderFormHtml += "</td>";
			
			orderFormHtml += "<td class =\"param\" width = \"60%\">";
			orderFormHtml += "<table  style=\"width:95%;border-bottom:0px;\">";
			NodeList params = doc.getElementsByTagName("param");
			int len = params.getLength();
			for (int j = 0; j < len; j++){
				
				if (params.item(j).getNodeType() == Node.ELEMENT_NODE){
					//System.out.println(params.item(j).getAttributes().getNamedItem("name").getNodeValue());
					//制造参数名称
					String paramName = params.item(j).getAttributes().getNamedItem("name").getNodeValue();;


					orderFormHtml += "<tr>";//内层table中的一行
					orderFormHtml += "<td width = \"40%\">";
					orderFormHtml += paramName;
					orderFormHtml += "</td>";
					
					orderFormHtml += "<td width = \"50%\">";
					//参数值类型，select|text
					
					//修改zhd
					String valueType = params.item(j).getAttributes().getNamedItem("type").getNodeValue().toUpperCase();
					ParamType type = ParamType.valueOf(valueType);
					
					//参数值类型为select
					switch(type){
					case TEXT:
						orderFormHtml += textHTML(paramName, params.item(j));
						break;
					case SELECT:
						orderFormHtml += selectHTML(paramName, params.item(j));
						break;
					case RADIO:
						orderFormHtml += radioHTML(paramName, params.item(j));
						break;
					case CHECKBOX:
						orderFormHtml += checkboxHTML(paramName, params.item(j));
						break;
					default:
						break;
					}
					//zhd
					
					orderFormHtml += "</td>";
					orderFormHtml += "</tr>";
					
				}
			}
			orderFormHtml += "</table>";
			
			orderFormHtml += "</td>";
			orderFormHtml += "</tr>";
			
			
			destroyXmlParser();
		}
		orderFormHtml += "</table>";
		
		return orderFormHtml;
	}
	//zhd
	
	//文本
	public String textHTML(String paramName, Node param){
		Node attr = null;
		String textHtml = null;
		
		attr = param.getAttributes().getNamedItem("value");
		if (attr != null){
			textHtml = "值：<input type =\"text\"  name = \"";
			textHtml += paramName;
			textHtml += "_value\" />";
		}
		else
		{
			textHtml = "最小值：<input type =\"text\"  name = \"";
			textHtml += paramName;
			textHtml += "_min\" />";
			
			textHtml += "最大值：<input type =\"text\"  name = \"";
			textHtml += paramName;
			textHtml += "_max\" />";
		}
		
		return textHtml;
	}
	
	//列表
	public String selectHTML(String paramName, Node param){
		String paramValue = null;//参数值
		String selectHtml = ("<select name = \"" + paramName +"\" width = \"40%\">");
		//参数可能的值
		NodeList values = param.getChildNodes();
		int valueNum = values.getLength();
		for (int k = 0; k < valueNum; k++){
			if (values.item(k).getNodeType() == Node.ELEMENT_NODE){
				paramValue = values.item(k).getAttributes().getNamedItem("value").getNodeValue();
				selectHtml += ("<option value = \"" + paramValue + "\">"
										+ paramValue + "</option>");
				
			}
		}
		
		selectHtml += "</select>";
		
		return selectHtml;
	}
	
	//多选按钮
	public String checkboxHTML(String paramName, Node param){
		String checkboxHtml = "";
		String paramValue = "";//radio可能的值
		//radio的选项
		NodeList values = param.getChildNodes();
		int valueNum = values.getLength();

		for (int k = 0; k < valueNum; k++){
			if (values.item(k).getNodeType() == Node.ELEMENT_NODE){
				paramValue = values.item(k).getAttributes().getNamedItem("value").getNodeValue();
				
				checkboxHtml += "<input type = \"checkbox\" name = \"";
				checkboxHtml += paramName;
				checkboxHtml += "\" value = \"";
				checkboxHtml += paramValue;
				checkboxHtml += "\" />";
				checkboxHtml += paramValue;
				checkboxHtml += "<br/>";
			}
		}
		return checkboxHtml;
	}
	
	//单选按钮
	public String radioHTML(String paramName, Node param){
		String radioHtml = "";
		String paramValue = "";//radio可能的值
		//radio的选项
		NodeList values = param.getChildNodes();
		int valueNum = values.getLength();
		for (int k = 0; k < valueNum; k++){
			if (values.item(k).getNodeType() == Node.ELEMENT_NODE){
				paramValue = values.item(k).getAttributes().getNamedItem("value").getNodeValue();
				
				radioHtml += "<input type = \"radio\" name = \"";
				radioHtml += paramName;
				radioHtml += "\" value = \"";
				radioHtml += paramValue;
				radioHtml += "\" />";
				radioHtml += paramValue;
				radioHtml += "<br/>";
				
			}
		}
		return radioHtml;
	}
	
}
