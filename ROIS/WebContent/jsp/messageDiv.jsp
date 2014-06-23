<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<%  
    request.setAttribute("decorator", "none");  
    //阻止浏览器缓存页面内容  
    response.setHeader("Cache-Control","no-cache"); //HTTP 1.1  
    response.setHeader("Pragma","no-cache"); //HTTP 1.0  
    response.setDateHeader ("Expires", 0);   
%>
<a href="<s:url action='messageBox' />"><s:property value="message"/></a>



