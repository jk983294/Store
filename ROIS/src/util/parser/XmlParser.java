package util.parser;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

import org.w3c.dom.Document;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

public class XmlParser {
	protected DocumentBuilderFactory domfac;
	protected DocumentBuilder dombuilder;
	protected InputStream is;
	protected Document doc;
	
	public XmlParser(){
		
		domfac=DocumentBuilderFactory.newInstance();  
		try {
			dombuilder=domfac.newDocumentBuilder();
		} catch (ParserConfigurationException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} 
	}
	
	//读入xml文档，建立xml文档树
	public void initNewXmlParser(String xmlURL){
		try{
			File f=new File(xmlURL);
			f.exists();
			is=new FileInputStream(xmlURL); 
			doc=dombuilder.parse(is);  
			
		} catch (FileNotFoundException e){  
			e.printStackTrace();  
		} catch (SAXException e){  
			e.printStackTrace();  
		} catch (IOException e){  
			e.printStackTrace();  
		}  
	}
	
	//关闭xml文档
	public void destroyXmlParser(){
		try {
			is.close();
		} catch (IOException e) {
			// TODO catch io exception
			e.printStackTrace();
		}
	}
	//获取某元素下所有子节点
	public NodeList getElements(String parentNodeName){
		
		
		return null;
	}
		
}
