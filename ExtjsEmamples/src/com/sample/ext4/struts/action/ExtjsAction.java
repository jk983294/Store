package com.sample.ext4.struts.action;

import java.util.ArrayList;
import java.util.List;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;
import org.apache.struts.actions.DispatchAction;

import com.sample.ext4.struts.bean.Person;
import com.sample.format.util.ExtHelper;


public class ExtjsAction extends DispatchAction {
	/*
	 * 获取XML格式的人员信息
	 */
	public ActionForward getPersonInfoXml(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
	throws Exception {
		response.setContentType("text/xml;charset=UTF-8");
		List persons = getPersons();
		//调用工具方法getXmlFromList将List转换为XML格式
		String xml = ExtHelper.getXmlFromList(persons.size(), persons);
		//将XML数据写回客户端
		response.getWriter().write(xml);
		return null;
	}
	/*
	 * 获取JSON格式的人员信息
	 */
	public ActionForward getPersonInfoJson(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
	throws Exception {
		response.setContentType("text/json;charset=UTF-8");
		//获取原始人员信息
		List persons = getPersons();
		//调用工具方法getJsonFromList将List转换为JSON格式
		String json = ExtHelper.getJsonFromList(persons.size(), persons);
		//将JSON数据写回客户端
		response.getWriter().write(json);
		return null;
	}
	/*
	 * 获取原始人员信息
	 */
	private List getPersons(){
		Person p1 = new Person("张三",29,"男","1979-09-13");
		Person p2 = new Person("李四",28,"男","1980-10-11");
		Person p3 = new Person("王五",27,"男","1981-01-23");
		List persons = new ArrayList();
		persons.add(p1);
		persons.add(p2);
		persons.add(p3);
		return persons;
	}
}