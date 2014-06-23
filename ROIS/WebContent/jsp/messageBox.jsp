<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@taglib prefix="s" uri="/struts-tags"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>我的信箱</title>
</head>
<body>
	<div id="messageBox">
		<table>
			<caption>我收到的消息</caption>
			<colgroup>
				<col></col>
				<col></col>
				<col width="65%"></col>
			</colgroup>
			<tbody>
				<tr>
					<th>MID</th>
					<th>类型</th>
					<th>内容</th>
				</tr>
				<s:iterator var="list" value="msgList" status="sta">
					<tr>
						<td>${list.mid}</td>
						<td>${list.type}</td>
						<td>${list.content}</td>
					</tr>
				</s:iterator>
			</tbody>
		</table>
	</div>
</body>
</html>