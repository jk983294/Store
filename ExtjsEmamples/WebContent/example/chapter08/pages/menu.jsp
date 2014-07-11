<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%
	String ctxpath = request.getContextPath();
%>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>书籍管理菜单</title>
<link href="../css/style.css" rel="stylesheet" type="text/css" />
</head>
<body>
<div class='menuDiv' style='height:96%'>
<table width=100%>
	<tr><td class='title'>功能菜单</td></tr>
	<tr>
		<td align=center>
			<a href="<%=ctxpath %>/book.do?method=showBookTypeList" target= "main" onclick="this.blur();" class="button">
				<span>书籍类型维护</span>
			</a>
		</td>
	</tr>
	<tr>
		<td align=center>
			<a href="<%=ctxpath %>/book.do?method=showBookList" target= "main" onclick="this.blur();" class="button">
				<span>书籍维护</span>
			</a>
		</td>
	</tr>
</table>
</div>
</body>
</html>