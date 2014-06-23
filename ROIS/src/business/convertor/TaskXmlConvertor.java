package business.convertor;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import org.apache.log4j.Logger;
import org.dom4j.Document;
import org.dom4j.Element;

import support.SrvcTaskMatch;
import business.entityBeans.TaskBean;

/**
 * 负责读取任务项的XML文档并解析为特定格式
 * @author Jersey
 * @date 2011-4-19
 */
public class TaskXmlConvertor {
	private static Logger log = Logger.getLogger(TaskXmlConvertor.class);

	/**
	   * 从dom4j Document 对象生成Task对应的HTML
	   * @param xmlString String  XML字符串
	   * @return HTML代码段
	   */
	public String toHtml(Document doc){
		String html="<table border='1' width='200px'>";
		Element root=doc.getRootElement();
		if(!root.getName().toLowerCase().equals("task")){
			return "不正确的XML格式!";
		}
		html+="<caption>【"+root.attributeValue("name")+"】详细参数信息</caption>";
		html+="<tbody><tr><th>参数</th><th>值</th></tr>";
	    Iterator<?> it =  root.elementIterator("param");
	    while(it.hasNext()){
	      Element ele = (Element)it.next();
	      String value=ele.attributeValue("value");
	      String min=ele.attributeValue("min");
	      String max=ele.attributeValue("max");
	      if(min!=null && max!=null){
	    	  value="最小值:"+min+" <br/>最大值:"+max;
	      }
	      html+="<tr><td>"+ele.attributeValue("name")+"</td><td>"+value+"</td></tr>";
	    }
	    html+="</tbody></table>"+"<span class='quantity'>数量："+root.attributeValue("quantity")+"</span>";
		return html;
	}
	/**
	 * 从{@link org.dom4j.Document}文档读取任务列表
	 * @param doc
	 * @return
	 */
	public List<TaskBean> toTaskList(Document doc){
		List<TaskBean> list=new ArrayList<TaskBean>();
		Element root=doc.getRootElement();
		if(!root.getName().toLowerCase().equals("tasklist")){
			log.error("任务列表XML文档格式错误！");
			return null;
		}
		Iterator<?> it =  root.elementIterator("task");
		while(it.hasNext()){
			Element ele=(Element)it.next();
			TaskBean tb=new TaskBean();
			tb.setTid(ele.attributeValue("id"));
			tb.setSname(SrvcTaskMatch.getSnameFromTname(ele.attributeValue("name")));
			tb.setDeadlineTime(ele.attributeValue("deadLineTime"));
			tb.setBidDeadline(ele.attributeValue("bidDeadLine"));
			tb.setUrl(ele.attributeValue("url"));
			list.add(tb);
		}
		return list;
	}

}
