<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@taglib prefix="s" uri="/struts-tags"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<link rel="stylesheet" type="text/css" href="/ROIS/css/default.css"
	media="screen">
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>Login to ROIS</title>
</head>
<body>
	<div id="main">
		<h2>
			登录ROIS<small>&nbsp;面向资源的集成服务器</small>
		</h2>
		<s:form action="loginEntp" namespace="/rois" theme="simple">
			<font color="red" size="2">${requestScope.tip}</font> 
			<p>
				<label for="username">Username:  </label>

				<s:textfield id="username" name="loginName" />
			</p>

			<p>
				<label for="password">Password:  </label>

				<s:password id="password" name="loginPwd" label="loginPassword" />
			</p>
			<p>
				<input type="submit" id="submit" value="登录" />
				&nbsp;<a href="entpRegister.jsp">现在注册</a>
			</p><s:if test="fieldErrors!=null&&fieldErrors.size !=0">
			<h4 class="alert"><s:fielderror /></h4></s:if>
		</s:form>
	</div>
	<div><a href="order/OrderIndexPage.jsp">@</a></div>
</body>
</html>