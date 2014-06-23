package backStage.communication;

import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

import org.dom4j.Document;
import org.dom4j.Element;

import util.XmlUtil;
/**
 * �ⴿ���Ǹ��м���̵Ĳ�����!
 * @deprecated
 * @author Jersey
 * @date 2011-4-19
 */
public class FakeJbpm {

	static private String biddingListXml="D:/BiddingXml/biddingXml_20110416022332.xml";
	
	static public void setBiddingListXml(String path){
		biddingListXml=path;
	}
	/**
	 * ����biddingList xml�ĵ�������bidWinnerMap,KeyΪTID��ValueΪwinner EID
	 * @return
	 */
	static public Map<String,String> parseBiddingList(){
		Map<String,String> map=new HashMap<String,String>();
		Document doc=XmlUtil.buildDocFromFile(biddingListXml);
		Element root = doc.getRootElement();
		Iterator<?> it=root.elementIterator("task");
		String tid="";
		String eid="";
		while(it.hasNext()){
			Element task=(Element) it.next();
			tid=task.attributeValue("id");
			Iterator<?> i=task.elementIterator("bidder");
			while(i.hasNext()){
				Element bidder=(Element) i.next();
				eid=bidder.attributeValue("id"); 
			}
			map.put(tid, eid);//�൱��ÿ�������һ��bidder�б�
		}
		return map;
	}
}
