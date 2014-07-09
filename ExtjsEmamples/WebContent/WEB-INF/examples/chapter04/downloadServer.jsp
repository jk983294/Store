<%@ page language="java" contentType="text/html; charset=utf-8" pageEncoding="utf-8"%>
<%
	String fileName = request.getParameter("fileName") + ".txt";
	response.setContentType("application/x-msdownload");
	response.setHeader("Content-disposition", "attachment; filename="+fileName);
	response.getWriter().write("下载文件内容。");
%>