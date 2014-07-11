<%@ page language="java" contentType="text/html; charset=utf-8" pageEncoding="utf-8"%>
<%@ page import="org.dom4j.*"%>
<%@ page import="org.dom4j.io.*"%>
<%
	SAXReader sax = new SAXReader();
	Document document = sax.read(request.getInputStream());
	Element root = document.getRootElement();
	String userName = root.element("userName").getStringValue();
	String password = root.element("password").getStringValue();
	String msg = "";
	if(userName.equals("tom") && password.equals("123")){
		msg = "登陆成功'xml方式'";
	}else{
		msg = "登陆失败'xml方式'";
	}
	response.getWriter().write(msg);
%>