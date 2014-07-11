<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<HTML>
<HEAD>
<TITLE>简易书籍管理系统</TITLE>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
</HEAD>
<frameset rows="100,*" cols="*" frameborder="NO" border="0" framespacing="0">
  <frame src="pages/title.jsp" name="title" scrolling="NO" noresize>
  <frameset id ="_content" cols="178,*" bordercolor="#2CA6DA" BORDER = "0">
      <FRAME SRC="pages/menu.jsp" NAME="tree" noresize SCROLLING="no" MARGINWIDTH=0 MARGINHEIGHT=0>
      <FRAME SRC="pages/about.jsp" NAME="main" noresize  SCROLLING="auto" MARGINWIDTH=0 MARGINHEIGHT=0>
  </frameset>
</frameset>
</HTML>