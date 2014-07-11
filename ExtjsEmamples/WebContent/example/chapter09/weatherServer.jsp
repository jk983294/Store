<%@ page language="java" contentType="text/html; charset=utf-8" pageEncoding="utf-8"%>
<%@ page import="java.util.*"%>
<%@ page import="java.io.*"%>
<%@ page import="org.dom4j.*"%>
<%@ page import="org.dom4j.io.*"%>
<%
    String cityCode = request.getParameter("cityCode"); //获取城市代码
    SAXReader reader = new SAXReader();                 //创建一个SAXReader
    //使用Reader解析远程服务器RSS信息
    Document doc = reader.read("http://xml.weather.yahoo.com/forecastrss?p="+cityCode+"&u=c");

    Element root = doc.getRootElement();
	Element channel = root.element("channel");

    Element windNode = channel.element("wind");
    Element atmosphereNode = channel.element("atmosphere");
    Element forecastNOde = channel.element("item").element("forecast");

    String windSpeed = "unknown";
    if(windNode != null){
    	windSpeed = windNode.attributeValue("speed")+"Kph";
    }
	String humidity = "unknown";
	if(atmosphereNode != null){
		humidity = atmosphereNode.attributeValue("humidity")+"%";
	}
	String maxTemp = "unknown";
	String minTemp = "unknown";
	String date = "unknown";
	if(forecastNOde != null){
		maxTemp = forecastNOde.attributeValue("high")+"℃";
		minTemp = forecastNOde.attributeValue("low")+"℃";
		date = forecastNOde.attributeValue("date");
	}
    //将信息拼接为JSON格式
    StringBuffer result = new StringBuffer("{");
    result.append("windSpeed:'" + windSpeed + "',");
    result.append("humidity:'" + humidity + "',");
    result.append("maxTemp:'" + maxTemp + "',");
    result.append("minTemp:'" + minTemp + "',");
	result.append("date:'" + date + "'");
    result.append("}");
    response.setContentType("text/json;charset=UTF-8");
	response.getWriter().write(result.toString());//输出天气信息   
%>