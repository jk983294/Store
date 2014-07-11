<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%
	String ctxpath = request.getContextPath();
%>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>新增书籍类型</title>
<link href="<%=ctxpath %>/examples/chapter08/css/style.css" rel="stylesheet" type="text/css" />
<script type="text/javascript">
	function submitForm(){
		var form = document.forms['addBookType'];
		if(form.title.value == ''){
			alert('类型名称为必填字段！');
		}else{
			form.submit();
		}
	}
	function back(){
		var form = document.forms['backForm'];
		form.submit();
	}
</script>
</head>
<body>
	<form name='backForm' action="book.do">
		<input type='hidden' name='method' value='showBookTypeList'/>
	</form>
	<form name='addBookType' action='book.do' method='POST'>
		<table width=100% height=100%>
			<tr>
				<td align = center>
				<br>
				<table class='tablecolor' width=50% height=50% border=0>
					<tr><td align=center colspan=2><br><h3>新增书籍类型</h3></td></tr>
					<tr><th class=title>类型名称：</th><td><input type=text name="title"/></td></tr>
					<tr><th class=title>类型说明：</th><td><textarea rows=5 name="detail"></textarea></td></tr>
					<tr><td colspan=2 align=right>
					<input type=button value="返回" onclick="back()"/>
					<input type=button value="提交数据" onclick="submitForm()"/>
					</td></tr>
				</table>
				</td>
			</tr>
		</table>
		<input type='hidden' name='method' value='addBookType'/>
	</form>
</body>
</html>