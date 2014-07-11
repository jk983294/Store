<%@ page language="java" contentType="text/html; charset=utf-8" pageEncoding="utf-8"%>
<%@ page import="net.sf.json.JSONObject" %>
<%@ page import="net.sf.json.JSONArray" %>
<%
String filter = request.getParameter("filter");
JSONArray filters = JSONArray.fromObject(filter);
int userId = filters.getJSONObject(0).getInt("value");
response.setContentType("application/json;charset=UTF-8");
String result = "{products:[{id : 1, title: '产品1', user_id : "+userId+"}, {id : 2, title: '产品2', user_id : "+userId+"}]}";
System.out.println("result="+result);
response.getWriter().write(result);
%>