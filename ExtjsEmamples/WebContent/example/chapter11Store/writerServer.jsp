<%@ page language="java" contentType="text/html; charset=utf-8" pageEncoding="utf-8"%>
<%@ page import="java.util.*" %>
<%@ page import="java.io.StringWriter" %>
<%@ page import="java.io.InputStreamReader" %>
<%@ page import="java.io.BufferedReader" %>
<%@ page import="org.apache.commons.io.IOUtils" %>
<%
InputStreamReader reader = new InputStreamReader(request.getInputStream());
List<String> lines = IOUtils.readLines(reader);
for(int i=0; i<lines.size(); i++){
	System.out.println(lines.get(i));
}
response.setContentType("application/json;charset=UTF-8");
response.getWriter().write("{success:true}");
%>