<%@ page language="java" contentType="text/html; charset=utf-8" pageEncoding="utf-8"%>
<%
	int value = Integer.parseInt(request.getParameter("value"));
	String target = request.getParameter("target");
	String result="";
	if(target.equals("s2")){
		switch(value){
			case 1:
				result = "[{name:'数学书籍',value:'1'},{name:'外语数据',value:'2'}]";
				break;
			case 2:
				result = "[{name:'程序设计',value:'3'},{name:'硬件维护',value:'4'}]";
				break;
		}
	}
	if(target.equals("s3")){
		switch(value){
			case 1:
				result = "[{name:'算术入门',value:'1'},{name:'高等数学',value:'2'}]";
				break;
			case 2:
				result = "[{name:'英语入门',value:'1'},{name:'日语入门',value:'2'}]";
				break;
			case 3:
				result = "[{name:'java基础',value:'1'},{name:'c++指南',value:'2'}]";
				break;
			case 4:
				result = "[{name:'电脑组装',value:'1'},{name:'硬件维修',value:'2'}]";
				break;
		}
	}
	response.getWriter().write(result);
%>