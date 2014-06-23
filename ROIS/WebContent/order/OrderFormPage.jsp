<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags"%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
<title>订单详情</title>
<link rel="stylesheet" type="text/css" href="../css/style.css"
	media="screen">
</head>

<body style="text-align: center;">
	<script type="text/javascript">
	function beforeSubmit() {
		var ordernum = document.getElementById("order_num").value;
		var flag;
		if (ordernum) 
			flag = true;
		else
		{
			flag = false;
			alert("请填写订单数量，最小为1");
		}
		return flag;
	}
	
</script>
	<div id="topbar">
		<div class="content wrap">
			<h1>欢迎使用ROIS集成服务平台</h1>
		</div>
	</div>
	<%
		String ordername = (String) session.getAttribute("ordername");
	%>

	<div id="subbar">
		<div class="content"></div>
	</div>
	<div class="content">
		<div id="main">
			<div class="padding">
				<h2><%=ordername%>
					订单详情
				</h2>
				<s:form method="post" action="submitorder"
					onsubmit="return beforeSubmit();">
					<jsp:useBean id="orderBean"
						class="business.entityBeans.OrderFormBean" scope="page">
						<%
							orderBean.setProcessName(ordername);
						%>
						<%=orderBean.getOrderFormHtml()%>
					</jsp:useBean>

					<br />
					<center>
						<s:textfield name="ordernumber" id="order_num" label="订单数量" />
					</center>
					<br />
					<s:submit value="确定" align="center" />
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
