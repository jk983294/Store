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
	
	//������дҳ��
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


	//��ó��������а�����������������б�
	public ArrayList<String> getServiceName(String ordername){
		services.clear();
		initNewXmlParser(XmlBasePath+ processName + xmlExt);
		//System.out.println("��ȡ���������������������"+ processName);
		
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
	
	//��ȡ����������ĳ���������Ķ���xml·��
	public String getServiceXmlUrl(String servicename){
		String serviceXmlUrl=GlobalParameter.ServiceUrlBiMap().get(servicename);
		return serviceXmlUrl;
	}
	
	//����������Ĳ��������б�
	//���ݷ������ƣ���ȡ������xml·����Ȼ���ȡ��������
	public ArrayList<String> getParamName(String servicename){

		String sXmlUrl = getServiceXmlUrl(servicename);
		params.clear();
		//sXmlUrl���Ѿ�����.xml�����Բ��ü�xmlExt
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
	
	//modified: 4.27 by zhd���ֿ�����text_value��min max
	public String getParamType(String serviceName, String paramName){
		String sXmlUrl = getServiceXmlUrl(serviceName);
		//sXmlUrl���Ѿ�����.xml�����Բ��ü�xmlExt
		initNewXmlParser(sXmlUrl);
		NodeList nodes = doc.getElementsByTagName("param");
		int nodeNum = nodes.getLength();
		String paramType = "";
		
		for (int i = 0; i < nodeNum; i++){
			if (nodes.item(i).getNodeType() == Node.ELEMENT_NODE){
				if (nodes.item(i).getAttributes().getNamedItem("name").getNodeValue().equals(paramName)){
					paramType = nodes.item(i).getAttributes().getNamedItem("type").getNodeValue();
					//text���͵Ĳ�����Ҫ���⴦��
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
		//���̶����ļ��ľ��Ե�ַ��Ŀǰֻ����ô����ȡ��Ե�ַ������
		initNewXmlParser(XmlBasePath+processName + xmlExt);

		//������ ����������������xml�ļ�
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
					//ȥ�����ո�
					//System.out.println(xmlUrlNodes.item(i).getFirstChild().getNodeValue().trim());
					String taskName=curNode.getAttributes().getNamedItem("name").getNodeValue().trim();
					XmlUrl = GlobalParameter.ServiceUrlBiMap().get(taskName);
			
					XmlUrls.add(XmlUrl);
				}
			}
		}
		destroyXmlParser();
		
		//һ����
		orderFormHtml = "<table border = '1' style=\"width:60%;margin:0 auto;\">";
		//�������춨��xml�ļ�
		int XmlSize = XmlUrls.size();
		String serviceName;//�����������
		for (int i = 0; i < XmlSize; i++){
			initNewXmlParser(XmlUrls.get(i));
			
			//�����������ɻ������ֵ�������������
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
					//�����������
					String paramName = params.item(j).getAttributes().getNamedItem("name").getNodeValue();;


					orderFormHtml += "<tr>";//�ڲ�table�е�һ��
					orderFormHtml += "<td width = \"40%\">";
					orderFormHtml += paramName;
					orderFormHtml += "</td>";
					
					orderFormHtml += "<td width = \"50%\">";
					//����ֵ���ͣ�select|text
					
					//�޸�zhd
					String valueType = params.item(j).getAttributes().getNamedItem("type").getNodeValue().toUpperCase();
					ParamType type = ParamType.valueOf(valueType);
					
					//����ֵ����Ϊselect
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
	
	//�ı�
	public String textHTML(String paramName, Node param){
		Node attr = null;
		String textHtml = null;
		
		attr = param.getAttributes().getNamedItem("value");
		if (attr != null){
			textHtml = "ֵ��<input type =\"text\"  name = \"";
			textHtml += paramName;
			textHtml += "_value\" />";
		}
		else
		{
			textHtml = "��Сֵ��<input type =\"text\"  name = \"";
			textHtml += paramName;
			textHtml += "_min\" />";
			
			textHtml += "���ֵ��<input type =\"text\"  name = \"";
			textHtml += paramName;
			textHtml += "_max\" />";
		}
		
		return textHtml;
	}
	
	//�б�
	public String selectHTML(String paramName, Node param){
		String paramValue = null;//����ֵ
		String selectHtml = ("<select name = \"" + paramName +"\" width = \"40%\">");
		//�������ܵ�ֵ
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
	
	//��ѡ��ť
	public String checkboxHTML(String paramName, Node param){
		String checkboxHtml = "";
		String paramValue = "";//radio���ܵ�ֵ
		//radio��ѡ��
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
	
	//��ѡ��ť
	public String radioHTML(String paramName, Node param){
		String radioHtml = "";
		String paramValue = "";//radio���ܵ�ֵ
		//radio��ѡ��
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
