<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%@ page import="java.util.List" %>
<%@ page import="com.sample.ext4.simplebook.bean.Book;" %>
<%
	String ctxpath = request.getContextPath();
%>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>书籍列表</title>
<link href="<%=ctxpath %>/examples/chapter08/css/style.css" rel="stylesheet" type="text/css" />
</head>
<script type="text/javascript">
	function deleteBook(bookId){
		var flag = window.confirm("您确定要删除所选书籍吗？");
		if(flag == true){
			var form = document.forms['delBook'];
			form.bookId.value = bookId;
			form.submit();
		}
	}
</script>
<body>
<form name = 'delBook' action="book.do">
	<input type='hidden' name='method' value='deleteBook'/>
	<input type='hidden' name='bookId'/>
</form>
<form name = 'addBook' action="book.do">
	<input type='hidden' name='method' value='toAddBook'/>
	<input type='submit' value='新增书籍'/>
</form>
<table width=100% class="linetable">
<tr class="linetitle">
<th>序号</th><th>书籍名称</th><th>作者</th><th>类型</th><th>金格</th><th>简介</th><th colspan=2>功能区</th>
</tr>
<%
	List books = (List)request.getAttribute("books");
	for(int i = 0 ; i < books.size() ; i++){
		Book book = (Book)books.get(i);
		String className = "line"+(i % 2);
%>
	<tr class="<%=className%>">
	<td><%=book.getId() %></td>
	<td><%=book.getBookName() %></td>
	<td><%=book.getAuthor() %></td>
	<td><%=book.getTypeName() %></td>
	<td><%=book.getPrice() %></td>
	<td><%=book.getBrief() %></td>
	<td align=center><a href="#" onclick="deleteBook(<%=book.getId()%>)">删除</a></td>
	<td align=center><a href="book.do?method=toModifyBook&bookId=<%=book.getId() %>">修改</a></td>
	</tr>
<%
	}
%>
</table>
</body>
</html>