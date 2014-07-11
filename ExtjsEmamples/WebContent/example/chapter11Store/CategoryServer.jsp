<%@ page language="java" contentType="text/html; charset=utf-8" pageEncoding="utf-8"%>
<%
String categoryId = request.getParameter("id");
response.setContentType("application/json;charset=UTF-8");
response.getWriter().write("{id : "+categoryId+", type: '产品类型"+categoryId+"'}");
%>