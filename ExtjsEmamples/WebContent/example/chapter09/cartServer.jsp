<%@ page language="java" contentType="text/html; charset=utf-8" pageEncoding="utf-8"%>
<%
	String flag = request.getParameter("flag");
	StringBuffer result = new StringBuffer();
	if(flag != null && flag.equals("goods")){
		result.append("[");
		result.append("{id:'goods1',name:'电脑',price:5000},");
		result.append("{id:'goods2',name:'内存',price:100},");
		result.append("{id:'goods3',name:'硬盘',price:600},");
		result.append("{id:'goods4',name:'键盘',price:30}");
		result.append("]"); 
		response.getWriter().write(result.toString());
		return;
	}
	String id = null;
	if(request.getParameter("id") != null){
		id = request.getParameter("id");
		response.getWriter().write("{id:'"+id+"',success:true}");
	}else{
		response.getWriter().write("{id:'"+id+"',success:false}");
	}
%>