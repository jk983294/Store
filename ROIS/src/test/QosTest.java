package test;

import java.util.ArrayList;
import java.util.List;
import business.entityBeans.ServiceBean;
import business.managers.ServiceSortManager;

public class QosTest {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		List<ServiceBean> s_list = new ArrayList<ServiceBean>();
		
		ServiceBean a = new ServiceBean("CPU1","service1.xml");
		ServiceBean b = new ServiceBean("CPU1","service2.xml");
		ServiceBean c = new ServiceBean("CPU1","service3.xml");
		s_list.add(a);
		s_list.add(b);
		s_list.add(c);
		
		ServiceSortManager ssm = new ServiceSortManager(s_list);
		ssm.Sort();
		System.out.println("a");

	}

}
