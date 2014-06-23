<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@taglib prefix="s" uri="/struts-tags"%>

<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>查看任务项</title>

</head>
<body>
	<s:url namespace="/rois" action="operateTask!bid" var="bidTaskLink" />
	<s:url  namespace="/rois" action="operateTask!reportStart" var="startTaskLink" />
	<s:url  namespace="/rois" action="operateTask!reportFinish" var="finishTaskLink" />
	<div id="taskList">
		<span class="error"><s:property value="taskListMsg" /> </span>
		<table>
			<caption>◆任务项列表</caption>
			<s:fielderror>
				<s:param>taskListMsg</s:param>
			</s:fielderror>
			<colgroup span="2">
			<col width="140px"></col><col></col>
			</colgroup>
			<colgroup span="5" width="90px">
			</colgroup>
			<colgroup span="2" width="80px">
			</colgroup>
			<tbody>
				<tr>
					<th>ID</th>
					<th>任务项</th>
					<th>发布时间</th>
					<th>竞标截止时间</th>
					<th>任务开始时间</th>
					<th>任务完成时间</th>
					<th>完成截止时间</th>
					<th>任务状态</th>
					<th>操作</th>
				</tr>
				<s:iterator var="task" value="taskList" status="st">
					<tr>
						<td><s:property value="#task.tid" />
						</td>
						<td><a
							href="<s:url action='viewTask!detail'/>?xmlUrl=${task.url}">${task.sname}</a>
						</td>
						<td><s:date name='#task.pubTime' format='yyyy-MM-dd hh:mm' />
						</td>
						<td><s:date name='#task.bidDeadline'
								format='yyyy-MM-dd hh:mm' /></td>
						<td><s:date name="#task.startTime" format='yyyy-MM-dd hh:mm' />
						</td>
						<td><s:date name="#task.task.endTime"
								format="yyyy-MM-dd hh:mm" />
						</td>
						<td><s:date name="#task.deadlineTime"
								format="yyyy-MM-dd hh:mm" /></td>
						<td><s:property value="#task.state" /></td>
						<td><a href="${bidTaskLink}?tid=${task.tid}">竞标</a>
						</td>
					</tr>
				</s:iterator>
			</tbody>
		</table>
	</div>
	<div id="myTaskList">
		<span class="error"> <s:property value="myTaskListMsg" /> </span>
		<table>
			<caption>◆我的任务项</caption>
			<s:fielderror>
				<s:param>myTaskListMsg</s:param>
			</s:fielderror>
			<colgroup span="2">
			<col width="140px"></col><col></col>
			</colgroup>
			<colgroup span="5" width="90px">
			</colgroup>
			<colgroup span="2" width="80px">
			</colgroup>
			<tbody>
				<tr>
					<th>ID</th>
					<th>任务项</th>
					<th>发布时间</th>
					<th>竞标截止时间</th>
					<th>任务开始时间</th>
					<th>任务完成时间</th>
					<th>完成截止时间</th>
					<th>任务状态</th>
					<th>操作</th>
				</tr>
				<s:iterator var="task" value="myTaskList" status="st">
					<tr>
						<td><s:property value="#task.tid" />
						</td>
						<td><a
							href="<s:url action='viewTask!detail'/>?xmlUrl=${task.url}">${task.sname}</a>
						</td>
						<td><s:date name='#task.pubTime' format='yyyy-MM-dd hh:mm' />
						</td>
						<td><s:date name='#task.bidDeadline'
								format='yyyy-MM-dd hh:mm' /></td>
						<td><s:date name="#task.startTime" format='yyyy-MM-dd hh:mm' />
						</td>
						<td><s:date name="#task.task.endTime"
								format="yyyy-MM-dd hh:mm" />
						</td>
						<td><s:date name="#task.deadlineTime"
								format="yyyy-MM-dd hh:mm" /></td>
						<td><s:property value="#task.state" /></td>
						<td><s:if test="#task.eid == #session.eid">
								<s:if test="#task.state == @support.TaskState@ACCEPTED ">
									<a href="${startTaskLink}?tid=${task.tid}">报告开始</a>
								</s:if>
								<s:if test="#task.state == @support.TaskState@RUNNING">
									<a href="${finishTaskLink }?tid=${task.tid}">报告结束</a>
								</s:if>
							</s:if> <s:else> 竞标中
									</s:else></td>
					</tr>
				</s:iterator>
			</tbody>
		</table>
	</div>
</body>
</html>