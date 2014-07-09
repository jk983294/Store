<%@ page language="java" contentType="text/html; charset=utf-8" pageEncoding="utf-8"%>
<%
	String password = request.getParameter("password");
	String msg = "";
	if(password.length() < 6){//密码不足6位验证失败
		msg = "{success:false,errors:{password:'密码不得小于六位数字'}}";
	}else{//验证成功
		msg = "{success:true}";
	}
	response.getWriter().write(msg);
%>