package com.voctor.midas.main;

import com.victor.midas.worker.common.TaskMgr;
import org.springframework.context.ApplicationContext;
import org.springframework.context.support.ClassPathXmlApplicationContext;
import org.springframework.context.support.FileSystemXmlApplicationContext;
import org.springframework.web.context.support.WebApplicationContextUtils;

/**
 * create db
 */
public class SpringMainDbCreation {

    public static void main(String[] args) {
        //ApplicationContext ctx = WebApplicationContextUtils.getRequiredWebApplicationContext(this.getServletContext());
        //ApplicationContext context = new FileSystemXmlApplicationContext("D:/GitHub/Store/midas/src/main/webapp/WEB-INF/midas-content.xml");

        //D:\GitHub\Store\midas\src\main\webapp\WEB-INF\midas-content.xml
        ApplicationContext context = new ClassPathXmlApplicationContext("/WEB-INF/midas-content.xml");
        TaskMgr taskMgr = (TaskMgr) context.getBean("taskMgr");
        taskMgr.cmd("bad cmd");
    }
}