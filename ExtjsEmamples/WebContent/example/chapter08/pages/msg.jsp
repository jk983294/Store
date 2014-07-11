<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>信息提示页面</title>
<link href="../css/style.css" rel="stylesheet" type="text/css" />
</head>
<body>
<table style="width: 100%; height: 100%" border=0>
	<tr>
		<td align=center><font color=yellow><b><%=request.getAttribute("message") %></b></font>
		<form name='back' action='book.do'>
		<input type='hidden' name='method' value='<%=request.getAttribute("method") %>'/>
		<input type=submit value='返回'/>
		</form>
		</td>
	</tr>
</table>
</body>
</html>