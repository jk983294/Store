<%@ page language="java" contentType="text/html; charset=utf-8" pageEncoding="utf-8"%>
<%@ page import="java.io.BufferedReader" %>
<%@ page import="net.sf.json.*"%>
<%
	BufferedReader in = request.getReader();
	StringBuffer jsonStr = new StringBuffer();
	String str = "";
	while((str=in.readLine())!=null){
		jsonStr.append(str);
	}
	JSONObject jsonObj = JSONObject.fromObject(jsonStr.toString());

	String userName = jsonObj.getString("userName");
	String password = jsonObj.getString("password");
	String msg = "";
	if(userName.equals("tom") && password.equals("123")){
		msg = "登陆成功'json方式'";
	}else{
		msg = "登陆失败'json方式'";
	}
	response.getWriter().write(msg);
%>