<%@ page language="java" contentType="text/html; charset=utf-8" pageEncoding="utf-8"%>
<%@ page import="java.util.*" %>
<%
String userId = request.getParameter("id");
response.setContentType("application/json;charset=UTF-8");
response.getWriter().write("{id : "+userId+", name: 'user"+userId+"'}");
%>