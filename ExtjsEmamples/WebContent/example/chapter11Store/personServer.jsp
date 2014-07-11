<%@ page language="java" contentType="text/html; charset=utf-8" pageEncoding="utf-8"%>
<%
	response.setContentType("application/json;charset=UTF-8");
	String result = "[{name: '张三', age : 20}, {name: '李四', age : 30}]";
	response.getWriter().write(result);
%>