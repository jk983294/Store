<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags"%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">

<%
	String ordername = (String) session.getAttribute("ordername");
	ArrayList<String> services = new ArrayList<String>();
	ArrayList<String> params = new ArrayList<String>();
%>
<html>
<head>
<title>订单结果</title>
<link rel="stylesheet" type="text/css" href="/ROIS/css/style.css"
	media="screen">
</head>

<body style="text-align: center;">
	<div id="topbar">
		<div class="content wrap">
			<h1>欢迎使用ROIS集成服务平台</h1>
		</div>
	</div>
	<div class="content">
		<div id="main">
			<div class="padding">
				<h2>订单已成功提交</h2>
				<a href="OrderIndexPage.jsp">返回</a>

			</div>

		</div>
		<div id="footer">
			<hr />
			<label>All rights reserved by seu.gdoc</label>
		</div>
	</div>
</body>
</html>
