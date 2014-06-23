<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@taglib prefix="s" uri="/struts-tags"%>
<%@taglib prefix="sx" uri="/struts-dojo-tags"%>

<div id="serviceDetail">
	<s:if test="#action.service!=null">
		<div class="extra">
			Node ID
			<s:property value="%{nodeId}" />
			&nbsp; &nbsp;| &nbsp; &nbsp; Node Name:
			<s:property value="%{nodeName}" />
		</div>
		<s:form id="serviceDetailForm" action="registerService"
			theme="simple" method="post">

			<h3>
				Service ID:
				<s:property value="%{service.sid}" />
				&nbsp;| &nbsp; Service Name:
				<s:property value="%{service.sname}" />
			</h3>

			<div class="innerHtml">
				<s:property value="html" escape="false" />
			</div>
			<s:hidden name="service.sid" value="%{service.sid}" />
			<s:hidden name="service.sname" value="%{service.sname}" />
			<s:hidden name="service.url" value="%{service.url}" />
			<s:submit value="注册服务" method="register" />

		</s:form>
	</s:if>
	<s:elseif test="#action.myService!=null">
		<s:form id="serviceDetailForm" action="registerService"
			theme="simple" method="post">
			<h3>
				Service ID:
				<s:property value="%{myService.sid}" />
				&nbsp;| &nbsp; Service Name:
				<s:property value="%{myService.sname}" />
			</h3>
			<div class="innerHtml">
				<s:property value="html" escape="false" />
			</div>
			<s:hidden name="myService.sid" value="%{myService.sid}" />
			<s:hidden name="myService.sname" value="%{myService.sname}" />
			<s:hidden name="myService.url" value="%{myService.url}" />
			<s:hidden name="myService.eid" value="%{myService.eid}" />

			<s:submit value="保存服务" method="modify"  />
			<s:submit value="注销服务" method="cancel" targets="serviceDetail" />
			
		</s:form>
	</s:elseif>
</div>