package business.entityBeans;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

import support.GlobalParameter;
/**
 * TODO:注释
 * @author zhd
 *
 */
public class OrderIndexBean{

	private static final long serialVersionUID = 1L;
	private String htmlText;
	
	
	public OrderIndexBean(){
		
	}
	
	public String getHtmlText() {
		return htmlText;
	}


	public void setHtmlText(String htmlText) {
		this.htmlText = htmlText;
	}
	
	public void InitOrderListPage(){
		htmlText = "<form><input type = textfield> </form>";
		
		
 		DocumentBuilderFactory domfac=DocumentBuilderFactory.newInstance();  
//
		try{  
			DocumentBuilder dombuilder=domfac.newDocumentBuilder();  
			//indexDemo.xml的绝对路径
			String xmlPath = GlobalParameter.OrderXmlDir()+"订单索引.xml";
			InputStream is=new FileInputStream(xmlPath);  
			
//			生成xml的解析树
			Document doc=dombuilder.parse(is);
			
//			root是XML文档的根结点
			NodeList categories = doc.getElementsByTagName("category");
			
			//依次遍历根结点的子节点服务目录
			int catLen = categories.getLength();

			
			if (categories != null){
				htmlText = "<table border='1' style=\"width:60%;margin:0 auto;\"> \n";//整个table
				
				for (int i = 0; i < catLen; i++)
				{
					//获取某个领域
					Node category = categories.item(i);
					
					if (category.getNodeType() == Node.ELEMENT_NODE){
						
						htmlText += "<tr> \n<td width = 30%> \n";//每个制造领域<tr><td>
						
						String field = category.getAttributes().getNamedItem("name").getNodeValue();
						//System.out.println("制造领域:"+field);
						
						htmlText += field;
						htmlText += "\n</td> \n";//每个制造领域名称</td>
						
						htmlText += "<td width = 70%> \n <table style=\"width:90%;border-bottom:0px;\">\n";//领域内订单
//						获取领域内所有制造订单
						NodeList orders = category.getChildNodes();
						
						int orderLen = orders.getLength();
						for (int j = 0; j < orderLen; j++){
							Node order = orders.item(j);
							
							
//							解析订单项order
							if (order.getNodeType() == Node.ELEMENT_NODE){
								
								String ordername = order.getAttributes().getNamedItem("name").getNodeValue();
								//System.out.println("\t订单名称:"+ordername);				
								
								htmlText += "<tr width = 100%> \n";//领域内制造订单<tr>
								
								htmlText += "<td width = 50% align =\"center\">";
								htmlText += ordername;
								htmlText += "</td>";
								
								htmlText += "<td width = 50%>";
								htmlText += "<input type = \"radio\" name = \"ordername\" value = \"";
								htmlText += ordername;
								htmlText += "\" align = \"right\" />";
								htmlText += "</td>";
								
								htmlText += "</tr> \n";
								
														
								
							}
							
						}//订单循环结束
						
						htmlText += "</table> \n ";
						
						htmlText += "</tr> \n";//每个制造领域tr
					}//领域内循环结束
					
					
				}
				htmlText += "</table> \n";//整个table
			}
				
		}catch (ParserConfigurationException e) {  
			e.printStackTrace();  
			} catch (FileNotFoundException e){  
				e.printStackTrace();  
			} catch (SAXException e){  
				e.printStackTrace();  
			} catch (IOException e){  
				e.printStackTrace();  
			}  
			
	}
	
}
