package util.reference;

import java.io.File;
import java.io.FileWriter;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.Reader;
import java.io.StringWriter;
import java.io.Writer;
import java.lang.reflect.Method;
import java.net.URL;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

import org.apache.log4j.Logger;
import org.dom4j.Document;
import org.dom4j.DocumentException;
import org.dom4j.DocumentHelper;
import org.dom4j.Element;
import org.dom4j.io.OutputFormat;
import org.dom4j.io.SAXReader;
import org.dom4j.io.XMLWriter;
@Deprecated
public class XmlUtils {
	 private static Logger log = Logger.getLogger(XmlUtils.class);

	  public static final String XML_HEAD_GBK = "<?xml version=\"1.0\" encoding=\"GBK\"?>";
	  public static final String XML_HEAD_GB2312 = "<?xml version=\"1.0\" encoding=\"GB2312\"?>";
	  public static final String XML_HEAD_UTF8 = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";

	  public static final String ATTR_CLASS = "_Class";          //��ʶJAVA�����������
	  public static final String ATTR_PARAM_TYPE = "_ParamType"; //��ʶ������JAVA����
	  public static final String ATTR_TYPE = "_AttrType";        //��ʶ���͵���������
	  public static final String ATTR_LIST_TAG = "_AttrLTag";    //��ʶList�и��������õ�Tag����������
	  public static final String TYPE_MAP = "map";               //���� Map
	  public static final String TYPE_LIST = "list";             //���� List
	  public static final String TYPE_PAIRLIST = "pairList";     //���� PairList
	  public static final String DEF_LIST_TAG = "li";            //Ĭ�� List �ĸ����Tag

	  public XmlUtils() {
	  }

	  /**
	   * ����һ���µ�Document
	   * @return Document
	   */
	  public static Document buildDocument(){
	    return DocumentHelper.createDocument();
	  }

	  /**
	   * ��XML�ַ������� dom4j document ����
	   * @param xmlString String  XML�ַ���
	   * @return Document
	   */
	  public static Document buildDocFromXML(String xmlString){
	    try{
	      return DocumentHelper.parseText(xmlString);
	    } catch(DocumentException ex){
	      log.error("�޷���XML�ִ�����dom4j Document:" + xmlString, ex);
	      return null;
	    }
	  }

	  /**
	   * ���ļ�����dom4j document ����
	   * @param fileName String   �ļ���(����·��)
	   * @return Document
	   */
	  public static Document buildDocFromFile(String fileName){
	    try{
	      SAXReader sb = new SAXReader();
	      return sb.read(new File(fileName));
	    }catch(Exception ex){
	      log.error("�޷����ļ�["+fileName+"]����dom4j document", ex);
	      return null;
	    }
	  }

	  /**
	   * ������������dom4j document����
	   * @param in InputStream
	   * @return Document
	   */
	  public static Document buildDocFromInputStream(InputStream in){
	    try{
	      SAXReader sb = new SAXReader();
	      return sb.read(in);
	    } catch(Exception ex){
	      log.error("�޷�������������dom4j document", ex);
	      return null;
	    }
	  }

	  /**
	   * ��Reader������dom4j document����
	   * @param rd Reader
	   * @return Document
	   */
	  public static Document buildDocFromReader(Reader rd){
	    try{
	      SAXReader sb = new SAXReader();
	      return sb.read(rd);
	    } catch(Exception ex){
	      log.error("�޷���Reader����dom4j document", ex);
	      return null;
	    }
	  }

	  /**
	   * ��URL������dom4j document����
	   * @param url URL
	   * @return Document
	   */
	  public static Document buildDocFromURL(URL url){
	    try{
	      SAXReader sb = new SAXReader();
	      return sb.read(url);
	    } catch(Exception ex){
	      log.error("�޷���URL["+ url +"]����dom4j document", ex);
	      return null;
	    }
	  }

	  /**
	   * ������bean������get������֯�Ľ��,����������ʽ��xml:
	   * <node><name><get1>val1</get1><get2>val2</get2></name></node>
	   * @deprecated
	   * @param node Element  �����
	   * @param name String   �������
	   * @param obj Object    bean����
	   */
	  public static void addChildByBean(Element node, String name, Object obj){
	    if(node == null || obj == null)
	      return;
	    Element pe = node.addElement(name);
	    Class<?> clazz = obj.getClass();
	    pe.addAttribute(ATTR_CLASS, clazz.getName());
	    Method[] methods = clazz.getDeclaredMethods();
	    Element e = null;
	    Method md = null;
	    String mname = null, aname = null, val = null;
	    int iAttr = 0;
	    for(int i = 0, l = methods.length; i < l; i++){
	      md = methods[i];
	      mname = md.getName();
	      clazz = md.getReturnType();
	      if(mname.startsWith("get") && md.getParameterTypes().length == 0 && !clazz.equals(Void.TYPE)){
	        try{
	          val = md.invoke(obj, null).toString();
	          aname = mname.substring(3);
	          e = pe.addElement(aname);
	          e.addAttribute(ATTR_PARAM_TYPE, clazz.getName());
	          e.setText(val);
	          iAttr ++;
	        } catch(Exception ex){
	        }
	      }
	    }
	    if(iAttr==0)
	      pe.setText(obj.toString());
	  }


	  /**
	  * ��List�е�PairEntry�������Node����Ϊ���ӽڵ�,���磺
	  * <Node><list[i].name>list[i].text</list[i].name></Node>
	  * @param node Element
	  * @param list PairList
	  */
	  public static void addChildByPairList(Element node, PairList list){
	    if(node == null || list == null || list.size() == 0)
	      return;
	    Element e = null;
	    PairEntry pair = null;
	    String name = null;
	    for(int i = 0, l = list.size(); i < l; i++){
	      pair = list.get(i);
	      name = pair.name;
	      if(pair.name != null){
	        name = pair.name.trim();
	        if(name.length() > 0){
	          e = node.addElement(pair.name);
	          if(pair.value != null)
	            e.setText(pair.value);
	        }
	      }
	    }
	  }

	  public static void addChildByList(Element node, String name, List<?> list){
	    addChildByList(node, name, list, true);
	  }

	  /**
	   * ��List��Ԫ����ӵ�Node����Ϊ���ӽڵ�,����:
	   * <Node><name>value1</name><name>value2</name></Node>
	   * @param node Element   ���ڵ�
	   * @param name String    �ӽڵ������
	   * @param list List      �ӽڵ��ֵ�б�
	   * @param bReloadable boolean     �Ƿ�������루���������XML����Ӷ����ǣ�
	   */
	  public static void addChildByList(Element node, String name, List<?> list, boolean bReloadable){
	    if(node == null || list == null || list.size() == 0)
	      return;
	    if(bReloadable){
	      node.addAttribute(ATTR_TYPE, TYPE_LIST);
	      node.addAttribute(ATTR_LIST_TAG, name);
	    }
	    Element e = null;
	    Object obj = null;
	    for(int i = 0, l = list.size(); i < l; i++){
	      e = node.addElement(name);
	      obj = list.get(i);
	      if(obj instanceof Map){
	        addChildByMap(e,(Map<?, ?>)obj, bReloadable);
	      } else if(obj instanceof List){
	        addChildByList(e, DEF_LIST_TAG, (List<?>)obj, bReloadable);
	      } else if(obj instanceof PairList){
	        addChildByPairList(e, (PairList)obj);
	      } else if(obj != null){
	        e.setText(obj.toString());
	      }
	    }
	  }

	  public static void addChildByMap(Element node, Map<?, ?> map){
	    addChildByMap(node, map, true);
	  }

	  /**
	   * ��Map�е�Ԫ����ӵ�Node����Ϊ���ӽڵ�, ����:
	   * <Node><key1>value1</key><key2>value2</key2>...</Node>
	   * @param node Element   ���ڵ�
	   * @param map Map        �ӽڵ����ֵ��
	   * @param bReloadable boolean     �Ƿ�������루���������XML����Ӷ����ǣ�
	   */
	  public static void addChildByMap(Element node, Map<?, ?> map, boolean bReloadable){
	    if(node==null || map==null || map.isEmpty())
	      return;
	    if(bReloadable)
	      node.addAttribute(ATTR_TYPE,TYPE_MAP);
	    Element e = null;
	    Object obj = null;
	    String name = null;
	    Map.Entry et = null;
	    Iterator<?> it = map.entrySet().iterator();
	    while(it.hasNext()){
	      et = ((Map.Entry)it.next());
	      name = et.getKey().toString();
	      e = node.addElement(name);
	      obj = et.getValue();
	      if(obj instanceof Map){
	        addChildByMap(e, (Map<?, ?>)obj, bReloadable);
	      } else if(obj instanceof List){
	        addChildByList(e, DEF_LIST_TAG, (List<?>)obj, bReloadable);
	      } else if(obj instanceof PairList){
	        addChildByPairList(e, (PairList)obj);
	      } else if(obj != null){
	        e.setText(obj.toString());
	      }
	    }
	  }

	  /**
	   * ��ȡָ���ڵ��������ӽڵ�������б�
	   * @param node Element
	   * @return List
	   */
	  public static List<String> getChildNodeNameList(Element node){
	    if(node==null)
	      return null;
	    List<String> list = new ArrayList<String>();
	    Iterator<?> it = node.elementIterator();
	    while(it.hasNext()){
	      Element ele = (Element)it.next();
	      list.add(ele.getName());
	    }
	    return list;
	  }

	  /**
	   * ��ȡָ���ڵ��£������ӽڵ��ֵ�б�
	   * @param node Element
	   * @return List
	   */
	  public static List<Object> getChildToList(Element node){
	    return getChildToList(node, null);
	  }

	  /**
	   * ��ȡָ���ڵ��£�ָ�����Ƶ������ӽڵ��ֵ�б�
	   * @param node Element
	   * @param name String    Ҫ��ƥ��Ľڵ����ƣ����Ϊnullƥ�����нڵ�
	   * @return List
	   */
	  public static List<Object> getChildToList(Element node, String name){
	    if(node==null)
	      return null;
	    List<Object> list = new ArrayList<Object>();
	    String attrType = null;
	    Iterator<?> it = (name==null)?node.elementIterator():node.elementIterator(name);
	    while(it.hasNext()){
	      Element ele = (Element)it.next();
	      attrType = ele.attributeValue(ATTR_TYPE);
	      if(TYPE_MAP.equals(attrType)){
	        list.add(getChildToMap(ele));
	      } else if(TYPE_LIST.equals(attrType)){
	        list.add(getChildToList(ele, ele.attributeValue(ATTR_LIST_TAG)));
	      } else if(TYPE_PAIRLIST.equals(attrType)){
	        list.add(getChildToPairList(ele));
	      } else{
	        list.add(ele.getText());
	      }
	    }
	    return list;
	  }

	  /**
	   * ��ȡָ���ڵ��£������ӽڵ����ֵ���ɵ�PairList�б�
	   * @param node Element
	   * @return List
	   */
	  public static PairList getChildToPairList(Element node){
	    if(node==null)
	      return null;
	    PairList list = new PairList();
	    Iterator<?> it = node.elementIterator();
	    while(it.hasNext()){
	      Element ele = (Element)it.next();
	      list.add(new PairEntry(ele.getName(),ele.getText()));
	    }
	    return list;
	  }

	  /**
	   * ��ȡָ���ڵ��µ������ӽڵ���ֵ���ɵ�Map���ݹ�����Map��
	   * @param node Element
	   * @return Map
	   */
	  public static Map<String, Object> getRecursiveMap(Element node){
	    if(node==null)
	      return null;
	    Map<String, Object> map = new LinkedHashMap<String, Object>();
	    Element ele = null;
	    Iterator<?> it = node.elementIterator();
	    while(it.hasNext()){
	      ele = (Element)it.next();
	      if(ele.elements().size()>0)
	        map.put(ele.getName(), getRecursiveMap(ele));
	      else
	        map.put(ele.getName(), ele.getText());
	    }
	    return map;
	  }

	  public static List<Object> getRecursiveList(Element node){
	    return getRecursiveList(node, null);
	  }

	  /**
	   * ��ȡָ���ڵ��µ������ӽڵ���ֵ���ɵ�List���ݹ�����List��
	   * @param node Element
	   * @param name String      //Ϊָ���Ľڵ���,Ϊ��Ϊ���нڵ�
	   * @return Map
	   */
	  public static List<Object> getRecursiveList(Element node, String name){
	    if(node == null)
	      return null;
	    List<Object> list = new ArrayList<Object>();
	    Element ele = null;
	    Iterator<?> it = (name == null) ? node.elementIterator() : node.elementIterator(name);
	    while(it.hasNext()){
	      ele = (Element)it.next();
	      if(ele.elements().size() > 0)
	        list.add(getRecursiveList(ele, name));
	      else
	        list.add(ele.getText());
	    }
	    return list;
	  }


	  /**
	   * ��ȡָ���ڵ��£������ӽڵ����ֵ���ɵ�Map
	   * @param node Element
	   * @return Map
	   */
	  public static Map<String, Object> getChildToMap(Element node){
	    if(node==null)
	      return null;
	    Map<String, Object> map = new LinkedHashMap<String, Object>();
	    Element ele = null;
	    String attrType = null;
	    Iterator<?> it = node.elementIterator();
	    while(it.hasNext()){
	      ele = (Element)it.next();
	      attrType = ele.attributeValue(ATTR_TYPE);
	      if(TYPE_MAP.equals(attrType)){
	        map.put(ele.getName(), getChildToMap(ele));
	      } else if(TYPE_LIST.equals(attrType)){
	        map.put(ele.getName(), getChildToList(ele, ele.attributeValue(ATTR_LIST_TAG)));
	      } else if(TYPE_PAIRLIST.equals(attrType)){
	        map.put(ele.getName(), getChildToPairList(ele));
	      } else{
	        map.put(ele.getName(), ele.getText());
	      }
	    }
	    return map;
	  }

	  /**
	   * ��ȡָ�������XML
	   * @param doc Document
	   * @param encoding String
	   * @return String
	   */
	  public static String toXMLString(Document doc, String encoding){
	    try{
	      StringWriter out = new StringWriter();
	      OutputFormat format = OutputFormat.createPrettyPrint();
	      format.setEncoding(encoding);
	      format.setLineSeparator("\r\n");
	      XMLWriter writer = new XMLWriter(out, format);
	      writer.write(doc);
	      writer.close();
	      return out.toString();
	    } catch(Exception ex){
	      log.error("��������,�޷����XML[" + encoding + "]", ex);
	      return "";
	    }
	  }

	  /**
	   * ����Ĭ�ϱ���(GB2312)���XML��OutputStream��
	   * @param doc Document
	   * @param out OutputStream
	   * @return boolean
	   */
	  public static boolean writeXML(Document doc, OutputStream out){
	    return writeXML(doc, out, "GB2312");
	  }

	  /**
	   * ���XML��OutputStream��
	   * @param doc Document
	   * @param out OutputStream
	   * @param encoding String
	   * @return boolean
	   */
	  public static boolean writeXML(Document doc, OutputStream out, String encoding){
	    try{
	      OutputFormat format = OutputFormat.createPrettyPrint();
	      format.setEncoding(encoding);
	      format.setLineSeparator("\r\n");
	      XMLWriter writer = new XMLWriter(out, format);
	      writer.write(doc);
	      writer.close();
	      return true;
	    } catch(Exception ex){
	      log.error("��������,�޷����XML[" + encoding + "]���������", ex);
	      return false;
	    }
	  }

	  /**
	   * ����Ĭ�ϱ���(GB2312)���XML��Writer��
	   * @param doc Document
	   * @param wr Writer
	   * @return boolean
	   */
	  public static boolean writeXML(Document doc, Writer wr){
	    return writeXML(doc, wr, "GB2312");
	  }

	  /**
	   * ���XML��Writer��
	   * @param doc Document
	   * @param wr Writer
	   * @param encoding String  ����
	   * @return boolean
	   */
	  public static boolean writeXML(Document doc, Writer wr, String encoding){
	    try{
	      OutputFormat format = OutputFormat.createPrettyPrint();
	      format.setEncoding(encoding);
	      format.setLineSeparator("\r\n");
	      XMLWriter writer = new XMLWriter(wr, format);
	      writer.write(doc);
	      writer.close();
	      return true;
	    } catch(Exception ex){
	      log.error("��������,�޷����XML[" + encoding + "]���������", ex);
	      return false;
	    }
	  }

	  /**
	   * ����Ĭ�ϱ���(GB2312)���XML���ļ���
	   * @param doc Document
	   * @param fileName String  �ļ���
	   * @return boolean
	   */
	  public static boolean writeXML(Document doc, String fileName){
	    return writeXML(doc, fileName, "GB2312");
	  }

	  /**
	   * ���XML���ļ���
	   * @param doc Document
	   * @param fileName String  �ļ���
	   * @param encoding String  ����
	   * @return boolean
	   */
	  public static boolean writeXML(Document doc, String fileName, String encoding){
	    try{
	      FileWriter fw = new FileWriter(fileName);
	      OutputFormat format = OutputFormat.createPrettyPrint();
	      format.setEncoding(encoding);
	      format.setLineSeparator("\r\n");
	      XMLWriter writer = new XMLWriter(fw, format);
	      writer.write(doc);
	      writer.close();
	      return true;
	    } catch(Exception ex){
	      log.error("��������,�޷����XML[" + encoding + "]���ļ�:" + fileName, ex);
	      return false;
	    }
	  }

	  //==== test ==============================
	  public static void main(String[] args){
	    Document doc = XmlUtils.buildDocument();
	    Element ele = doc.addElement("Root");
	    addChildByBean(ele, "BEAN", new Integer(10));
	    System.out.println("=====================");
	    System.out.println(XmlUtils.toXMLString(doc,"GB2312"));
	  }

}
