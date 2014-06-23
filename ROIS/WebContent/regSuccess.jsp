<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>注册成功</title>
<link rel="stylesheet" type="text/css" href="/ROIS/css/default.css"
	media="screen">
<script language="JavaScript1.2" type="text/javascript">
<!--
	//  Place this in the 'head' section of your page.    

	function delayURL(url) {
		var delay = document.getElementById("time").innerHTML;
		//alert(delay);  
		if (delay > 0) {
			delay--;
			document.getElementById("time").innerHTML = delay;
		} else {
			window.top.location.href = url;
		}
		setTimeout("delayURL('" + url + "')", 1000); //delayURL(http://wwer)  
	}
//-->
</script>
</head>
<body>
	<div id="main">

		<h2>
			企业注册成功! &nbsp;<span id="time">5</span><small>秒钟后自动跳转到登陆界面</small>
		</h2>
		
		<p>如果页面未跳转，请点击：
			<a href="entpLogin.jsp">企业用户登陆</a>
			<script type="text/javascript">
				delayURL("entpLogin.jsp");
			</script>
		</p>
	</div>
</body>
</html>