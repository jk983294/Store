<%@ page language="java" contentType="text/html; charset=utf-8" pageEncoding="utf-8"%>
<%
	response.setContentType("application/xml;charset=UTF-8");
	StringBuffer personName = new StringBuffer();
	personName.append("<users>");
	personName.append("<user>");
	personName.append("<id>1</id>");
	personName.append("<name>张三</name>");
	personName.append("</user>");
	personName.append("</users>");
	System.out.println(personName.toString());
	response.getWriter().write(personName.toString());
%>