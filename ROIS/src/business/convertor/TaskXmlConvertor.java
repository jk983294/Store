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
 * �����ȡ�������XML�ĵ�������Ϊ�ض���ʽ
 * @author Jersey
 * @date 2011-4-19
 */
public class TaskXmlConvertor {
	private static Logger log = Logger.getLogger(TaskXmlConvertor.class);

	/**
	   * ��dom4j Document ��������Task��Ӧ��HTML
	   * @param xmlString String  XML�ַ���
	   * @return HTML�����
	   */
	public String toHtml(Document doc){
		String html="<table border='1' width='200px'>";
		Element root=doc.getRootElement();
		if(!root.getName().toLowerCase().equals("task")){
			return "����ȷ��XML��ʽ!";
		}
		html+="<caption>��"+root.attributeValue("name")+"����ϸ������Ϣ</caption>";
		html+="<tbody><tr><th>����</th><th>ֵ</th></tr>";
	    Iterator<?> it =  root.elementIterator("param");
	    while(it.hasNext()){
	      Element ele = (Element)it.next();
	      String value=ele.attributeValue("value");
	      String min=ele.attributeValue("min");
	      String max=ele.attributeValue("max");
	      if(min!=null && max!=null){
	    	  value="��Сֵ:"+min+" <br/>���ֵ:"+max;
	      }
	      html+="<tr><td>"+ele.attributeValue("name")+"</td><td>"+value+"</td></tr>";
	    }
	    html+="</tbody></table>"+"<span class='quantity'>������"+root.attributeValue("quantity")+"</span>";
		return html;
	}
	/**
	 * ��{@link org.dom4j.Document}�ĵ���ȡ�����б�
	 * @param doc
	 * @return
	 */
	public List<TaskBean> toTaskList(Document doc){
		List<TaskBean> list=new ArrayList<TaskBean>();
		Element root=doc.getRootElement();
		if(!root.getName().toLowerCase().equals("tasklist")){
			log.error("�����б�XML�ĵ���ʽ����");
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
