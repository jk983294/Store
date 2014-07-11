<%@ page language="java" contentType="text/html; charset=utf-8" pageEncoding="utf-8"%>
<%
    String id = request.getParameter("id");
	String personName = "{id:"+id+",name:'张三',age:30}";
	boolean jsonP  = false;
	//获取回调函数名称
	String cb = request.getParameter("callback");
	if (cb != null) {
		jsonP  = true;
		response.setContentType("text/javascript;charset=UTF-8");
	} else {
		response.setContentType("application/x-json;charset=UTF-8");
	}
	String msg = "";
	if (jsonP ) {
		msg = cb + "(";
	}

	msg += personName;

	if (jsonP ) {
		msg += ");";
	}
	response.getWriter().write(msg);
%>