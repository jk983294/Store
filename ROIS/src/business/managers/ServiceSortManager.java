package business.managers;

import java.util.*;
import org.dom4j.Document;
import org.dom4j.Element;
import org.w3c.dom.NodeList;

import util.XmlUtil;

import business.entityBeans.ParameterBean;
import business.entityBeans.ServiceBean;

public class ServiceSortManager {
	private List<ServiceBean> s_list;
	private List<QosBean> list;
	int element_num;
	int qos_entry_num;
	min_max record[];

	public ServiceSortManager(List<ServiceBean> s_list) {
		this.s_list = s_list;
		list = new ArrayList<QosBean>();
		record =new min_max[8];
	}

	public void AddQos(QosBean s) {
		list.add(s);
	}

	public List<ServiceBean> getList() {
		return s_list;
	}

	public void setS_List(List<ServiceBean> list) {
		this.s_list = list;
	}

	public void Sort() {		
		element_num = s_list.size();
		float qos[] = new float[element_num];
		creatParaList();
		Calculate_Min_Max();
		for (int i = 0; i < element_num; i++) {
			for (int j = 0; j < 4; j++) {	
				ParameterBean temp = getPara(i,j);
				float qos_item_min = ConvertValue(j*2+0,temp.getMin(),temp.isType());
				float qos_item_max = ConvertValue(j*2+1,temp.getMax(),temp.isType());
				qos[i] += (qos_item_min+qos_item_max)/2;
			}
			s_list.get(i).setQos_value(qos[i]);
		}
		//ComparatorService comparator=new ComparatorService();
		//Collections.sort(list, comparator);
		for (int i = 0; i < element_num; i++){
			System.out.println(s_list.get(i).getQos_value());
		}
	}

	public void creatParaList(){
		XmlUtil xmlutil = new XmlUtil();
		
		for (int i = 0; i < element_num; i++) {
			Document doc = xmlutil.buildDocFromFile(s_list.get(i).getUrl());
			if(doc==null)
				System.out.println("no file");
			List listRowSet=doc.selectNodes("//service/qos/time");
			
			
			//Element timeElement=rootElement.getElement("qos");
			creatPara(listRowSet);
			/*NodeList time =doc.getElementsByTagName("time"); 
			for(Iterator j = time.elementIterator(); j.hasNext();){   
				Element node=(Element) j.next();   
				System.out.println(node.getName()+":"+node.getText());   
			}*/
			
		}
	}
	public ParameterBean creatPara(List element){
		String min;
		String max;
		String unit;
		String tendency;
		String type;
		for(Iterator it=element.iterator();it.hasNext();)
		{
			Element timeElement=(Element)it.next();
			type=timeElement.attributeValue("type");
			if(type.equals("number")){
				min=timeElement.attributeValue("min");
				max=timeElement.attributeValue("max");
				unit=timeElement.attributeValue("unit");
				tendency=timeElement.attributeValue("tendency");
			}
			else{
				min=timeElement.attributeValue("scope");
				max=timeElement.attributeValue("value");
			}
			
		}
		/*NodeList  nodes  =  doc.getElementsByTagName("time");  
        for(int  i=0;i<nodes.getLength();i++){  
        	Element  node=(Element)  nodes.item(i);  
            String  name=node.getElementsByTagName("姓名".item(0).getFirstChild().getNodeValue();  
            String  value=node.getElementsByTagName("职业".item(0).getFirstChild().getNodeValue();            
        } 
        /*String min = element.attribute("min");
		String max = max;
		String unit = unit;
		String tendency = tendency;
		String type = type;*/
		ParameterBean a= new ParameterBean(1,1,"s",true,true);
		return a;		
	}
	
	public void Calculate_Min_Max() {		
		for (int i = 0; i < 4; i++) {
			record[i*2+0]=new min_max();
			record[i*2+1]=new min_max();
			ParameterBean temp = getPara(0,i);
			record[i*2+0].min = temp.getMin();
			record[i*2+0].max = temp.getMin();
			record[i*2+1].min = temp.getMax();
			record[i*2+1].max = temp.getMax();
			for(int j = 0; j < element_num; j++){
				ParameterBean temp1 =  getPara(j,i);
				if(record[i*2+0].min>temp1.getMin()){
					record[i*2+0].min = temp1.getMin();
				}
				if(record[i*2+0].max<temp1.getMin()){
					record[i*2+0].max=temp1.getMin();
				}
				if(record[i*2+1].min>temp1.getMax()){
					record[i*2+1].min = temp1.getMax();
				}
				if(record[i*2+1].max<temp1.getMax()){
					record[i*2+1].max=temp1.getMax();
				}
			}
		}	
	}
	ParameterBean getPara(int qos_counter,int qos_para_counter){
		switch(qos_para_counter){
		case 0: 
			return list.get(qos_counter).time;
		case 1: 
			return list.get(qos_counter).quality;
		case 2: 
			return list.get(qos_counter).cost;
		case 3: 
			return list.get(qos_counter).throughput;
		}
		return null;
	}
	
	/*数值归一化 
	i为第几个属性  
	value为待处理值 
	tendency = true 正项 tendency = false 逆向
	*/
	public float ConvertValue(int i,float value,boolean tendency){
		if(tendency){//正项
			if(record[i].max == record[i].min){
				return 1;
			}
			else{
				return (value - record[i].min)/(record[i].max-record[i].min);
			}
		}
		else{//逆
			if(record[i].max == record[i].min){
				return 1;
			}
			else{
				return (record[i].max - value)/(record[i].max-record[i].min);
			}
		}
	}
}

class min_max {
	float min;
	float max;
}

class QosBean {
	//private List<ItemBean> ItemList;
	ParameterBean time;
	ParameterBean quality;
	ParameterBean cost;
	ParameterBean throughput;
	
	QosBean(ParameterBean time, ParameterBean quality,
			ParameterBean cost, ParameterBean throughput) {
		this.time = time;
		this.quality = quality;
		this.cost = cost;
		this.throughput = throughput;
	}	
}
