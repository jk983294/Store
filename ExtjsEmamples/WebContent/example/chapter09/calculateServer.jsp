<%@ page language="java" contentType="text/html; charset=utf-8" pageEncoding="utf-8"%>
<%@ page import="org.apache.commons.jexl.*"%>
<%
	String jexlExp = request.getParameter("Exp");
	System.out.println(jexlExp);
	Expression e = ExpressionFactory.createExpression(jexlExp);
	JexlContext jc = JexlHelper.createContext();
	Object result = e.evaluate(jc);
	response.getWriter().write(result.toString());
%>