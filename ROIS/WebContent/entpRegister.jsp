<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@taglib prefix="s" uri="/struts-tags"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<link rel="stylesheet" type="text/css" href="/ROIS/css/default.css"
	media="screen">
<title>Enterprise Register</title>
<script src="/ROIS/js/jquery.js" type="text/javascript"></script>
<script src="/ROIS/js/jquery.validate.js" type="text/javascript"></script>
<script type="text/javascript">
	$(document).ready(function() {
		$('label.required').prepend('&nbsp;<strong>*</strong>&nbsp;');
		jQuery.validator.messages.required = "";
		$("form").validate({
			invalidHandler : function(e, validator) {
				var errors = validator.numberOfInvalids();
				if (errors) {
					var message = '您忘了填写' + errors + '个必填内容.它们已经被高亮显示';
					$("div.error span").html(message);
					$("div.error").show();
				} else {
					$("div.error").hide();
				}
			},
			onkeyup : false,
			debug : false
		});

	});
</script>
</head>
<body>
	<div id="main">
		<h2>
			注册ROIS<small>&nbsp;企业用户</small>
		</h2>
		<s:form action="registerEntp" namespace="/rois" theme="simple">
			<div class="error">
				<img src="../images/warning.gif" alt="Warning!" width="24"
					height="24" style="float: left; margin: -5px 10px 0px 0px;">
				<span><s:fielderror />
				</span>
			</div>

			<p>
				<label class="required">企业名称:</label>
				<s:textfield cssClass="required" name="enterprise.ename" />
			</p>
			<p>
				<label class="required">登录用户名:</label>
				<s:textfield cssClass="required" name="enterprise.loginName" />
			</p>
			<p>
				<label class="required">登录密码:</label>
				<s:password id="password" cssClass="required"
					name="enterprise.loginPwd" />
			</p>
			<p>
				<label class="required">确认密码:</label>
				<s:password cssClass="required" name="repassword" />
			</p>
			<p>
				<label>企业地址:</label>
				<s:textfield name="enterprise.location" />
			</p>
			<p>
				<label>电子邮箱:</label>
				<s:textfield name="enterprise.email" />
			</p>
			<p>
				<label>电话:</label>
				<s:textfield name="enterprise.phone" />
			</p>
			<p>
				<label>其他描述:</label>
				<s:textarea name="enterprise.desc" />
			</p>
			<p>
				<s:submit id="submit" value="注册" />
				&nbsp;<a href="entpLogin.jsp">用已有账户登录</a>
			</p>
		</s:form>
	</div>
</body>
</html>