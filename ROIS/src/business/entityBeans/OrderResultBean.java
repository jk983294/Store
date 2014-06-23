package business.entityBeans;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;

import javax.servlet.ServletContext;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpSession;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.OutputKeys;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerConfigurationException;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;

import org.apache.struts2.ServletActionContext;
import org.w3c.dom.Document;
import org.w3c.dom.Element;

import business.convertor.OrderXmlConvertor;

import support.GlobalParameter;
import util.TimeUtil;

/**
 * TODO:注释
 * 
 * @author zhd
 * 
 */
public class OrderResultBean {
	private HttpServletRequest request;
	private String xmlFile;
	private String orderName;
	private String resultHtml;

	// 订单包含的制造服务名称和制造服务的参数
	private OrderXmlConvertor convertor;
	private ArrayList<String> services;
	private ArrayList<String> params;

	// 必须提供public类型的构造函数
	public OrderResultBean() {
		resultHtml = "";
		convertor = new OrderXmlConvertor();
	}

	
	public void setHttpRequest(HttpServletRequest request) {
		this.request = request;
	}
	
	public void setOrderName(String orderName){
		this.orderName = orderName;
	}
	
	public String getXmlFile() {
		return xmlFile;
	}

	public void setXmlFile(String xmlFile) {
		this.xmlFile = xmlFile;
	}

	
	// 生成结果页面html代码
	public String getResultHtml() {
		resultHtml = "";
		convertor.setProcessName(orderName);
		// 获取订单下的所有服务名
		services = convertor.getServiceName(orderName);
		int serviceNum = services.size();
		int paramNum = 0;

		// -------------
		// 参数名|参数值
		// -------------
		resultHtml += "<table border= 1 style='width:60%;margin:0 auto;'>";

		resultHtml += "<tr style='font-weight:bold;font-size:16px;border-bottom:2px solid #3F4040;'>";
		resultHtml += "<td width = \"40%\">";
		resultHtml += "订单名称";
		resultHtml += "</td>";
		resultHtml += "<td>";
		resultHtml += orderName;
		resultHtml += "</td>";
		resultHtml += "</tr>";
		
		for (int i = 0; i < serviceNum; i++) {
			// System.out.println("\t订单下制造服务名称:" + services.get(i));
			resultHtml += "<tr>";
			resultHtml += "<td width = \"40%\">";// 参数名
			resultHtml += "<span style=\"font-weight:bold; font-size: 12px\">";
			resultHtml += services.get(i);
			resultHtml += "</span>";
			resultHtml += "</td>";

			resultHtml += "<td>";
			// 获取某个服务所有的参数名称
			params = convertor.getParamName(services.get(i));
			paramNum = params.size();
			//这边需要加判断，看是否为text类型的
			for (int j = 0; j < paramNum; j++) {
				String param = params.get(j);
				resultHtml += "<div>";
				resultHtml += param;
				resultHtml += ":<br/>";
				String paramValue = "";
				//两种text文本value min max
				if (convertor.getParamType(services.get(i), param).equals("text_value")){
					paramValue = (String) request.getParameter(param+"_value");
					resultHtml += paramValue;
					
				}else if (convertor.getParamType(services.get(i), param).equals("text_min_max")){
					resultHtml += "最小值：";
					paramValue = (String) request.getParameter(param+"_min");
					resultHtml += paramValue;
					resultHtml += "<br/>最大值：";
					paramValue = (String) request.getParameter(param+"_max");
					resultHtml += paramValue;
				}else if (convertor.getParamType(services.get(i), param).equals("checkbox")){
					String []paramValues = request.getParameterValues(param);
					int size = paramValues.length;
					for (int k = 0; k < size; k++){
						resultHtml += paramValues[k];
						if (k < size-1){
							resultHtml += " ,";
						}
					}
				}else {
					paramValue = (String) request.getParameter(param);
					resultHtml += paramValue;
				}

				resultHtml += "</div>";
			}
			resultHtml += "</td>";
			resultHtml += "</tr>";
		}

		resultHtml += "<tr style='font-weight:bold;font-size:16px;border-top:2px solid #3F4040;'>";
		resultHtml += "<td width = \"40%\">";
		resultHtml += "订单数：";
		resultHtml += "</td>";
		resultHtml += "<td>";
		resultHtml += (String) request.getParameter("ordernumber");
		resultHtml += "</td>";
		resultHtml += "</tr>";
		resultHtml += "</table>";

		return resultHtml;
	}

	// 生成订单xml文档，首先需要调用setSession方法，以获得订单名称
	public void writeOrderXml() {
		DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
		try {
			DocumentBuilder db = dbf.newDocumentBuilder();
			Document doc = db.newDocument();
			Element root = doc.createElement("order");
			
			root.setAttribute("name", orderName);
			String orderUser = (String) request.getParameter("username");
			String orderNumber = (String) request.getParameter("ordernumber");
			// 日期格式
			String orderDateTime = TimeUtil.getNowString("yyyy-MM-dd hh:mm:ss");

			root.setAttribute("name", orderName);
			root.setAttribute("user", orderUser);
			root.setAttribute("quantity", orderNumber);
			root.setAttribute("time", orderDateTime);

			doc.appendChild(root);

			convertor.setProcessName(orderName);
			services = convertor.getServiceName(orderName);
			int serviceNum = services.size();
			for (int i = 0; i < serviceNum; i++) {
				Element requirement = doc.createElement("requirement");
				requirement.setAttribute("item", services.get(i));
				root.appendChild(requirement);

				params = convertor.getParamName(services.get(i));
				int paramNum = params.size();
				for (int j = 0; j < paramNum; j++) {
					Element param = doc.createElement("param");
					param.setAttribute("name", params.get(j));
					//@modified by zhd 4.27
					String paramType = convertor.getParamType(services.get(i), params.get(j));
					if (paramType.equals("text_value")){
						param.setAttribute("type", "text");
						param.setAttribute("value", request.getParameter(params.get(j)+"_value"));
	
					}else if (paramType.equals("text_min_max")){
						param.setAttribute("type", "text");
						param.setAttribute("min", request.getParameter(params.get(j)+"_min"));
						param.setAttribute("max", request.getParameter(params.get(j)+"_max"));

					}else if (paramType.equals("checkbox")){
						param.setAttribute("type", paramType);
						
						String []paramValues = request.getParameterValues(params.get(j));
						String checkboxValues = "";
						int size = paramValues.length;
						for (int k = 0; k < size; k++){
							checkboxValues += paramValues[k];
							if (k < size - 1){
								checkboxValues += ",";
							}
						}
						param.setAttribute("value", checkboxValues);
					}else{
						param.setAttribute("type", paramType);
						param.setAttribute("value", request.getParameter(params.get(j)));
					}
					requirement.appendChild(param);
					
				}
			}

			// 写xml内容
			TransformerFactory tff = TransformerFactory.newInstance();
			Transformer tf = null;

			tf = tff.newTransformer();
			tf.setOutputProperty(OutputKeys.INDENT, "yes");
			tf.setOutputProperty(OutputKeys.ENCODING, "UTF-8");

			DOMSource ds = new DOMSource(doc);
			
			String tempName = (String) request.getSession().getAttribute("username")
					+ TimeUtil.getNowString()
					+ orderName + ".xml";
			this.xmlFile = GlobalParameter.OrderXmlDir()+ tempName;
			File file = new File(xmlFile);
			if (!file.exists()) {
				file.createNewFile();
			}
			FileOutputStream fs = new FileOutputStream(file);
			// BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(
			// file, "UTF-8"));

			StreamResult result = new StreamResult(fs);

			tf.transform(ds, result);

			// file.close();
			System.out.println("OrderResultBean: 关闭文件");

		} catch (ParserConfigurationException e) {
			e.printStackTrace();
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		} catch (TransformerConfigurationException e) {
			e.printStackTrace();
		} catch (TransformerException e) {
			e.printStackTrace();
		}
	}

}
