<%@ page language="java" contentType="text/html; charset=utf-8" pageEncoding="utf-8"%>
<%@ page import="java.text.SimpleDateFormat"%>
<%@ page import="java.util.Date"%>
<%
	SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd" + " " + "hh:mm:ss"); 
	Date date = new Date();
	String time = dateFormat.format(date);
	response.getWriter().write(time);
%>