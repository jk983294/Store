<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>

<%@ page
	import="org.w3c.dom.*,org.xml.sax.*,java.io.*,javax.xml.parsers.*"%>
<%@ taglib prefix="s" uri="/struts-tags"%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
<title>订单首页</title>
<link rel="stylesheet" type="text/css" href="../css/style.css"
	media="screen">
<script language="javascript" type="text/javascript">
function validate(){
	var selected=false;
	var ordernames=document.forms[0].ordername;
	for(i=0;i<ordernames.length;i++){
		if(ordernames[i].checked){
			selected=true;
		}
	}
	if(!selected){
		alert("请选择一个订单!");
		return false;
	}
	return true;
}
</script>
</head>

<body style="text-align: center;">
	<div id="topbar">
		<div class="content wrap">
			<h1>欢迎使用ROIS集成服务平台</h1>
		</div>
	</div>
	<div id="subbar">
		<div class="content">
			<%
				//登陆界面获取用户名称，放入session中
				String username = request.getParameter("username");
				if (username != null) {
					session.setAttribute("username", username);
				} else {
					username = (String) session.getAttribute("username");
				}
			%>
			用户名:
			<%=username%>
		</div>
	</div>
	<div class="content">
		<div id="main">
			<div class="padding">
				<h2>订单列表</h2>
				<s:form method="post" onsubmit="return validate();"
					action="selectorder" namespace="/rois">
					<jsp:useBean id="orderList"
						class="business.entityBeans.OrderIndexBean" scope="page">
						<%
							orderList.InitOrderListPage();
						%>
						<%=orderList.getHtmlText()%>
					</jsp:useBean>

					<div align="center">
						<s:submit value="下订单" align="center" />
					</div>
				</s:form>
			</div>

		</div>
		<div id="footer">
			<hr />
			<label>All rights reserved by seu.gdoc</label>
		</div>
	</div>
</body>
</html>
