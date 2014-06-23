<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%><%@ taglib prefix="s" uri="/struts-tags"%>

<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>Administration Page</title>
</head>
<body>
	<div id="serviceIndexUpload">
		<h1>Service Index Xml File Upload</h1>

		<s:actionerror />
		<s:fielderror />
		<s:form action="upload" method="POST" enctype="multipart/form-data">
			<s:radio name="importType" list="{'ServiceIndex','OrderIndex'}" />
			<s:file name="upload" label="File" />
			<s:submit />
		</s:form>
	</div>
</body>
</html>