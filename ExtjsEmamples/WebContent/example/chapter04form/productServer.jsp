<%@ page language="java" contentType="text/html; charset=utf-8" pageEncoding="utf-8"%>
<%
//获取产品id
String productId = request.getParameter("productId");
if("001".equals(productId)){
	String msg = "{success:true,data:{introduction:'本产品美观实用，售后服务优秀。'}}";
	response.getWriter().write(msg);
}else{
	String msg = "{success:false,errorMessage:'数据不存在'}";
	response.getWriter().write(msg);
}
%>