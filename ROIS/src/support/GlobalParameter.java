package support;

import java.net.URISyntaxException;

import com.google.common.collect.BiMap;
import com.google.common.collect.HashBiMap;

/**
 * 提供全局变量的类
 * 
 * @OPTIMIZE_ME ：考虑以后用XML配置或者properties的方法？
 * @author Jersey
 * @date 2011-4-19
 */
public class GlobalParameter {
	private static final String SERVICE_XML_DIR="xmlFile/ServiceDefine/";
	private static final String SERVICE_INDEX_FILEPATH = "xmlFile/ServiceDefine/index.service.xml";
	private static final String SERVICE_Entp_XML_DIR = "xmlFile/Service_EntpXML/";
	private static final String BIDDING_XML_DIR = "xmlFile/BiddingXml/";
	private static final String RECEIVE_FILE_DIR = "xmlFile/Receive_File_Dir/";
	private static final String JBPM_IP = "10.3.17.20";
	private static final String ORDER_XML_DIR = "xmlFile/OrderXML/";
	
	public static String ServiceDir(){
		String rootPath=null;
		try {
			rootPath=getWebRoot();
		} catch (IllegalAccessException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (URISyntaxException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return rootPath+SERVICE_XML_DIR;
	}
	private static BiMap<String,String> serviceUrlMap;
	/**
	 * Return a BiMap which Key is Service Name and value is Service_def_url
	 * @return
	 */
	public static BiMap<String,String> ServiceUrlBiMap(){
		if(serviceUrlMap==null)
			serviceUrlMap=HashBiMap.create(); 
		return serviceUrlMap;
	}
	public static String OrderXmlDir() {
		String rootPath=null;
		try {
			rootPath=getWebRoot();
		} catch (IllegalAccessException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (URISyntaxException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return rootPath+ORDER_XML_DIR;
	}

	public static String JbpmIP() {
		return JBPM_IP;
	}

	public static String RecvFileDir() {
		String rootPath=null;
		try {
			rootPath=getWebRoot();
		} catch (IllegalAccessException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (URISyntaxException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return rootPath+RECEIVE_FILE_DIR;
	}

	public static String BiddingXmlDir() {
		String rootPath=null;
		try {
			rootPath=getWebRoot();
		} catch (IllegalAccessException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (URISyntaxException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return rootPath+BIDDING_XML_DIR;
	}

	public static String SrvEntpXmlDir() {
		String rootPath=null;
		try {
			rootPath=getWebRoot();
		} catch (IllegalAccessException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (URISyntaxException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return rootPath+SERVICE_Entp_XML_DIR;
	}

	public static String SrvIndexFilePath() {
		String rootPath=null;
		try {
			rootPath=getWebRoot();
		} catch (IllegalAccessException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (URISyntaxException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return rootPath+SERVICE_INDEX_FILEPATH;
	}

	public static String getWebClassesPath() throws URISyntaxException {
		String path =GlobalParameter.class.getResource("/").toURI().getPath();
		return path;

	}

	public static String getWebInfPath() throws IllegalAccessException, URISyntaxException {
		String path = getWebClassesPath();
		if (path.indexOf("WEB-INF") > 0) {
			path = path.substring(0, path.indexOf("WEB-INF") + 8);
		} else {
			throw new IllegalAccessException("路径获取错误");
		}
		return path;
	}

	public static String getWebRoot() throws IllegalAccessException, URISyntaxException {
		String path = getWebClassesPath();
		if (path.indexOf("WEB-INF") > 0) {
			path = path.substring(0, path.indexOf("WEB-INF/classes"));
		} else {
			throw new IllegalAccessException("路径获取错误");
		}
		return path;
	}

	public void testPath() throws IllegalAccessException, URISyntaxException {
		System.out.println("Web Class Path = " + getWebClassesPath());
		System.out.println("WEB-INF Path = " + getWebInfPath());
		System.out.println("WebRoot Path = " + getWebRoot());
	}
}
