<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%@ page import="java.util.List" %>
<%@page import="com.sample.ext4.simplebook.bean.BookType;"%>
<%
	String ctxpath = request.getContextPath();
%>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>新增书籍</title>
<link href="<%=ctxpath %>/examples/chapter08/css/style.css" rel="stylesheet" type="text/css" />
<script type="text/javascript">
	function submitForm(){ //提交表单数据
		var form = document.forms['addBook'];//得到'addBook'表单的引用
		if(form.bookName.value == ''){//验证书籍名称是否为空
			alert('书籍名称为必填字段！');//如果为空则提示用户
		}else if(form.author.value == ''){//验证书籍作者是否为空
			alert('书籍作者为必填字段！'); //如果为空则提示用户
		}else{
			form.submit();//提交表单数据
		}
	}
	function back(){//返回书籍列表页面
		var form = document.forms['backForm'];
		form.submit();
	}
</script>
</head>
<body>
	<form name='backForm' action="book.do">
		<input type='hidden' name='method' value='showBookList'/>
	</form>
	<form name='addBook' action='book.do' method='POST'>
		<table width=100% height=100%>
			<tr>
				<td align = center>
				<br>
				<table class='tablecolor' width=50% height=50% border=0>
					<tr><td align=center colspan=2><br><h3>新增书籍</h3></td></tr>
					<tr><th class=title>书籍名称：</th>
					<td><input type=text name="bookName"/></td></tr>
					<tr><th class=title>作者姓名：</th>
					<td><input type=text name="author"/></td></tr>
					<tr><th class=title>所属类型：</th><td>
					<select name="bookTypeId">
					<%
						List bookTypes = (List)request.getAttribute("bookTypes");
						for(int i = 0 ;i < bookTypes.size() ; i++){
							BookType bookType = (BookType)bookTypes.get(i);
					%>
						<option value=<%=bookType.getId()%>><%=bookType.getTitle()%></option>
					<%
						}
					%>
					</select>
					</td></tr>
					<tr><th class=title>购买金额：</th>
					<td><input type=text name="price"/></td></tr>
					<tr><th class=title>书籍简介：</th>
					<td><textarea rows=5 name="brief"></textarea></td></tr>
					<tr><td colspan=2 align=right>
					<input type=button value="返回" onclick="back()"/>
					<input type=button value="提交数据" onclick="submitForm()"/>
					</td></tr>
				</table>
				</td>
			</tr>
		</table>
		<input type='hidden' name='method' value='addBook'/>
	</form>
</body>
</html>