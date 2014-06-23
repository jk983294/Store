<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@ taglib uri="http://www.opensymphony.com/sitemesh/decorator"
	prefix="decorator"%>
<%@ taglib uri="http://www.opensymphony.com/sitemesh/page" prefix="page"%>
<%@ taglib prefix="s" uri="/struts-tags"%>
<%@ taglib prefix="sx" uri="/struts-dojo-tags"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<link rel="stylesheet" type="text/css" href="/ROIS/css/style.css"
	media="screen">
<title><decorator:title default="ROIS" /></title>
<sx:head />
<decorator:head />
</head>
<body>
	<div id="topbar">
		<div class="content wrap">
			<h1>WelCome to ROIS!</h1>
		</div>
	</div>

	<div id="subbar">
		<div class="content">
			<div id="navMenu" class="menu left">
				<s:if test="#session['loginName']!=null">
					<s:url action="messageBox" var="messageBoxLink" />
					<s:url action="displayServiceRegister"
						var="displayServiceRegisterLink" />
					<s:url action="viewTask!taskTable" var="viewTaskLink" />
					<ul>
						<li><a href="${viewTaskLink}">查看任务项</a></li>
						<li><a href="${displayServiceRegisterLink}">注册服务</a></li>
						<li><a href="${messageBoxLink}">我的信箱</a>
						</li>
						<li><a href="<s:url action="logout"/>">登出</a>
						</li>

					</ul>
					<sx:div cssClass="wrap right" id="messageDiv"
						href="messageDiv.action" updateFreq="3000" delay="3000"
						errorText="加载消息数据出错" loadingText="正在消息内容">    提示信息的地方</sx:div>
				</s:if>
			</div>

		</div>
	</div>
	<div class="content">
		<div id="main">
			<div class="padding">
				<s:if test="#session['loginName']==null">
					<h2>您还没有登录！</h2>
			请<a href="/ROIS/entpLogin.jsp">登录</a>或者<a href="/ROIS/entpRegister.jsp">注册新用户</a>
				</s:if>
				<s:else>
					<decorator:body />
				</s:else>
			</div>
		</div>
		<div id="footer">
		<hr/>
		<label>All rights reserved by seu.gdoc</label>
		</div>
	</div>
</body>
</html>