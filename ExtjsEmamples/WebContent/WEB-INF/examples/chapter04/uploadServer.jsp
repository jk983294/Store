<%@ page language="java" contentType="text/html; charset=utf-8" pageEncoding="utf-8"%>
<%@ page import="org.apache.commons.fileupload.*"%>
<%@ page import="org.apache.commons.fileupload.FileItem"%>
<%
DiskFileUpload upload = new DiskFileUpload();
upload.setHeaderEncoding("utf8");
java.util.List items = upload.parseRequest(request);
java.util.ListIterator listIterator = items.listIterator();
String fileName = "";
while(listIterator.hasNext()){
	FileItem item = (FileItem)listIterator.next();
	if(!item.isFormField()){
		fileName = item.getName();
		fileName = fileName.substring(fileName.lastIndexOf("\\")+1);//从全路径中提取文件名
	}
}
String msg = "{success:true,file:'"+fileName+"'}";
response.getWriter().write(msg);
%>