<%@ page language="java" contentType="text/html; charset=utf-8" pageEncoding="utf-8"%>
<%
	String userName = request.getParameter("userName");
	String password = request.getParameter("password");
	String msg = "";
	if(userName.equals("tom") && password.equals("123")){
		msg = "登陆成功";
	}else{
		msg = "登陆失败";
	}
	response.getWriter().write(msg);
%>