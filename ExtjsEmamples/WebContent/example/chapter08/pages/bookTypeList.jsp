<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%@ page import="java.util.List" %>
<%@ page import="com.sample.ext4.simplebook.bean.BookType;" %>
<%
	String ctxpath = request.getContextPath();
%>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<link href="<%=ctxpath %>/examples/chapter08/css/style.css" rel="stylesheet" type="text/css" />
<title>书籍类型列表</title>
</head>
<script type="text/javascript">
	function deleteBookType(bookTypeId){
		var flag = window.confirm("您确定要删除所选书籍类型吗？");
		if(flag == true){
			var form = document.forms['delBookType'];
			form.bookTypeId.value = bookTypeId;
			form.submit();
		}
	}
</script>
<body>
<form name = 'delBookType' action="book.do">
	<input type='hidden' name='method' value='deleteBookType'/>
	<input type='hidden' name='bookTypeId'/>
</form>
<form name='addBookTypeForm' action="book.do">
	<input type='hidden' name='method' value='toAddBookType'/>
	<input type='submit' value='新增书籍类型'/>
</form>
<%
	List bookTypes = (List)request.getAttribute("bookTypes");
%>
<table width=100% class="linetable">
<tr class="linetitle">
<th>序号</th><th>类型名称</th><th>类型说明</th><th colspan=2>功能区</th>
</tr>
<%
	for(int i = 0 ; i < bookTypes.size() ; i++){
		BookType bookType = (BookType)bookTypes.get(i);
		String className = "line"+(i % 2);
%>
<tr class="<%=className%>">
<td><%=bookType.getId() %></td>
<td><%=bookType.getTitle() %></td>
<td><%=bookType.getDetail() %></td>
<td align=center><a href="#" onclick="deleteBookType(<%=bookType.getId()%>)">删除</a></td>
<td align=center><a href="book.do?method=toModifyBookType&bookTypeId=<%=bookType.getId() %>">修改</a></td>
</tr>
<%
	}
%>
</table>
</body>
</html>