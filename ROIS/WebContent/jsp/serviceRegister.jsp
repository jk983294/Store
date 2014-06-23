<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@ taglib prefix="s" uri="/struts-tags"%>
<%@ taglib prefix="sx" uri="/struts-dojo-tags"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>注册服务</title>
<%-- <sx:head debug="true" cache="false" compressed="false"/> --%>
<script src="../js/jquery.js" type="text/javascript"></script>
<script src="../js/jquery.validate.js" type="text/javascript"></script>

<script language="JavaScript" type="text/javascript">
	function treeNodeSelected(node) {
		if (node.node.children.length > 0) {
			node.node.expand();
			return;
		}
		dojo.io.bind({
			url : "<s:url value='displayServiceDetail.action'/>?nodeId="
					+ node.node.widgetId,
			load : function(type, data, evt) {
				var divDisplay = dojo.byId("showDetailDiv");
				divDisplay.innerHTML = data;
			},
			mimeType : "text/html"
		});
	};
	dojo.event.topic.subscribe("treeSelected", treeNodeSelected);

	function myService(xmlUrl) {
		alert(xmlUrl);
		dojo.io.bind({
			url : "<s:url value='displayMyServiceDetail.action'/>?xmlUrl="
					+ xmlUrl,
			load : function(type, data, evt) {
				var divDisplay = dojo.byId("showDetailDiv");
				divDisplay.innerHTML = data;
			},
			mimeType : "text/html"
		});
	};
</script>

</head>
<body>
	<div id="showDetailDiv" class="right_side"></div>
	<div id="left_side">
		<h3>服务列表</h3>
		<sx:tree id="ServiceList" rootNode="root" nodeTitleProperty="name"
			nodeIdProperty="id" childCollectionProperty="children"
			treeSelectedTopic="treeSelected" />
	</div>


	<div id="myServices">
		<s:actionerror cssClass="error" />
		<s:if test="#action.actionErrors.size()==0">
			<h3>已经注册的服务</h3>
			<ul>
				<s:iterator var="list" value="myServices" status="sta">
					<s:url action="displayMyServiceDetail" var="mySrvDetailLink">
						<s:param name="sid" value="#list.sid" />
					</s:url>
					<li><sx:a href="%{mySrvDetailLink}" errorText="系统服务器返回信息出错"
							targets="showDetailDiv">${list.sname}</sx:a></li>
				</s:iterator>
			</ul>
		</s:if>
	</div>
	<!--用来撑开mainDiv的  -->
	<div class="clear"></div>
</body>
</html>