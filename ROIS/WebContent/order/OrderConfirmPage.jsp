<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags"%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">

<html>
<head>
<title>订单结果</title>
<link rel="stylesheet" type="text/css" href="../css/style.css"
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
				<h2>订单详细要求</h2>
					<s:property value = "orderDetailHtml" escape = "false"/>
 
					<br />

					<center>
						<s:form method="post" action="sendorder">
							<input type="hidden" name="orderXmlName"
								value="${resultBean.xmlFile}" />
							<s:submit value="确认并提交" />
						</s:form>
					</center>

			</div>

		</div>
		<div id="footer">
			<hr />
			<label>All rights reserved by seu.gdoc</label>
		</div>
	</div>
</body>
</html>
