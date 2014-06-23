package business.managers;

import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.Queue;

import org.dom4j.Document;
import org.dom4j.Element;

import support.GlobalParameter;
import support.StrutsTreeNode;
import support.StrutsTreeNodeManager;
import util.XmlUtil;
import business.convertor.EnterpriseListConvertor;
import business.convertor.ServiceListConvertor;
import business.convertor.SrvEntpListConvertor;
import business.entityBeans.EnterpriseBean;
import business.entityBeans.ServiceBean;
import business.entityBeans.SrvEntpBean;
import dao.ServiceDao;
import dao.db.BatchedParameters;
import dao.db.DBType;
import dao.db.FixedParameters;
import dao.db.MySqlParameter;
import dao.db.Parameters;

public class ServiceManager extends BaseManager implements ServiceDao {
	@Override
	public int addServices(List<ServiceBean> list) throws SQLException {
		String sql = "INSERT INTO TSERVICES(SID,SNAME,serviceParamXML) VALUES(?,?,?)";
		Parameters parms = new BatchedParameters(3, list.size());
		for (ServiceBean sb : list) {
			parms.add(new MySqlParameter(DBType.Int32, sb.getSid()));
			parms.add(new MySqlParameter(DBType.String, sb.getSname()));
			parms.add(new MySqlParameter(DBType.String, sb.getUrl()));
		}
		return getDb().ExecuteNoneQuery(sql, parms);
	}

	@Override
	public int registerService(SrvEntpBean se) throws SQLException {
		String sql = "INSERT INTO MAP_SRV_ENTP (SID,EID,ENTPSRVCXML) VALUES(?,?,?)";
		Parameters parms = new FixedParameters(3);
		parms.add(new MySqlParameter(DBType.Int32, se.getSid()));
		parms.add(new MySqlParameter(DBType.Int32, se.getEid()));
		parms.add(new MySqlParameter(DBType.String, se.getUrl()));

		return getDb().ExecuteNoneQuery(sql, parms);
	}

	@Override
	public ServiceBean queryByName(String sname) throws SQLException {
		String sql = "SELECT SID,SNAME,SERVICEPARAMXML FROM TSERVICES WHERE SNAME=?";
		Parameters parms = new FixedParameters(1);
		parms.add(new MySqlParameter(DBType.String, sname));
		List<ServiceBean> list = getDb().ExecuteObject(
				new ServiceListConvertor(), sql, parms);

		return list.size() == 0 ? null : list.get(0);
	}

	@Override
	public ServiceBean queryBySid(String sid) throws SQLException {
		String sql = "SELECT SID,SNAME,SERVICEPARAMXML FROM TSERVICES WHERE SID=?";
		Parameters parms = new FixedParameters(1);
		parms.add(new MySqlParameter(DBType.Int32, sid));
		List<ServiceBean> list = getDb().ExecuteObject(
				new ServiceListConvertor(), sql, parms);

		return list.size() == 0 ? null : list.get(0);
	}

	@Override
	public List<ServiceBean> queryAllServices() throws SQLException {
		String sql = "SELECT * FROM TSERVICES";
		return getDb().ExecuteObject(new ServiceListConvertor(), sql, null);
	}

	@Override
	public List<EnterpriseBean> queryEntpBySid(String sid) throws SQLException {
		String sql = "SELECT * FROM TENTERPRISES TE WHERE TE.EID IN "
				+ "(SELECT MAP.EID FROM MAP_SRV_ENTP MAP WHERE SID =?)";
		Parameters parms = new FixedParameters(1);
		parms.add(new MySqlParameter(DBType.Int32, sid));
		return getDb().ExecuteObject(new EnterpriseListConvertor(), sql, parms);
	}

	@Override
	public List<ServiceBean> querySrvcByEid(String eid) throws SQLException {
		String sql = "SELECT * FROM TServices TS WHERE TS.SID IN "
				+ "(SELECT MAP.SID FROM MAP_SRV_ENTP MAP WHERE EID =?)";
		Parameters parms = new FixedParameters(1);
		parms.add(new MySqlParameter(DBType.Int32, eid));
		return getDb().ExecuteObject(new ServiceListConvertor(), sql, parms);
	}

	@Override
	public List<SrvEntpBean> querySrvEntpByEid(String eid) throws SQLException {
		String sql = "SELECT * FROM MAP_SRV_ENTP MAP,TSERVICES S WHERE MAP.SID=S.SID AND EID =?";
		Parameters parms = new FixedParameters(1);
		parms.add(new MySqlParameter(DBType.Int32, eid));
		return getDb().ExecuteObject(new SrvEntpListConvertor(), sql, parms);

	}

	@Override
	public SrvEntpBean querySrvEntpByEidSid(String eid, String sid)
			throws SQLException {
		String sql = "SELECT * FROM MAP_SRV_ENTP MAP,TSERVICES S WHERE MAP.SID=S.SID AND EID =? AND MAP.SID=?";
		Parameters parms = new FixedParameters(2);
		parms.add(new MySqlParameter(DBType.Int32, eid));
		parms.add(new MySqlParameter(DBType.Int32, sid));
		List<SrvEntpBean> list = getDb().ExecuteObject(
				new SrvEntpListConvertor(), sql, parms);
		return list.size() == 0 ? null : list.get(0);

	}
	@Override
	public int deleteSrvEntp(SrvEntpBean seb)throws SQLException{
		String sql="DELETE FROM MAP_SRV_ENTP WHERE SID=? AND EID=?";
		Parameters parms = new FixedParameters(2);
		parms.add(new MySqlParameter(DBType.Int32, seb.getSid()));
		parms.add(new MySqlParameter(DBType.Int32, seb.getEid()));
		return getDb().ExecuteNoneQuery(sql, parms);
	}

	@Override
	public List<SrvEntpBean> querySrvEntpBySid(String Sid) throws SQLException {
		String sql = "SELECT * FROM MAP_SRV_ENTP MAP WHERE SID =?";
		Parameters parms = new FixedParameters(1);
		parms.add(new MySqlParameter(DBType.Int32, Sid));
		return getDb().ExecuteObject(new SrvEntpListConvertor(), sql, parms);

	}

	/**
	 * ����Service�б�Ӧ����WEB SERVER����ʱ������
	 * 
	 * @param serviceIndexXml
	 */
	public static List<ServiceBean> importServices(String serviceIndexXml) {
		List<ServiceBean> list = new ArrayList<ServiceBean>();
		// 1.����XML�ĵ�ΪList<ServiceBean>,����д�����ݿ�
		// 2.ת��index.xmlΪStrutsTreeNode��ʽ
		Document doc = XmlUtil.buildDocFromFile(serviceIndexXml);
		Element root = doc.getRootElement();
		/* �������root�����ӽڵ� */
		// ��ʼ��
		int nodeId = Integer.parseInt(StrutsTreeNodeManager.ROOT_ID);
		Queue<Element> queue = new LinkedList<Element>();
		Queue<StrutsTreeNode> queue2 = new LinkedList<StrutsTreeNode>();
		StrutsTreeNode rootNode = new StrutsTreeNode(nodeId++,
				root.attributeValue("name"));
		StrutsTreeNodeManager.addNode(rootNode.getId(), rootNode);
		for (Iterator<?> i = root.elementIterator(); i.hasNext();) {
			Element ele = (Element) i.next();
			queue.offer(ele);
			// ����StrutsTree
			StrutsTreeNode nodeNew = new StrutsTreeNode(nodeId++,
					ele.attributeValue("name"));
			StrutsTreeNodeManager.addNode(nodeNew.getId(), nodeNew);
			rootNode.addChildren(nodeNew);
			queue2.offer(nodeNew);
		}

		// ��ʼ����
		while (queue.size() > 0) {
			Element ele = queue.poll();
			StrutsTreeNode node = queue2.poll();
			String name = ele.attributeValue("name").trim();

			if (ele.getName().toLowerCase().equals("service")) {
				String url = ele.attributeValue("url").trim();
				// �������IF�����������ų��������ݵ�
				if (null != url && !"".equals(url)) {
					url = GlobalParameter.ServiceDir() + url;
					list.add(new ServiceBean(name, url));
					// ����Service Name��url��ӳ�䣬��OrderXmlConvertorʹ��
					GlobalParameter.ServiceUrlBiMap().put(name, url);

				}
			} else if (ele.getName().toLowerCase().equals("category")) {
				for (Iterator<?> i = ele.elementIterator(); i.hasNext();) {
					Element child = (Element) i.next();
					queue.offer(child);

					StrutsTreeNode nodeNew = new StrutsTreeNode(nodeId++,
							child.attributeValue("name"));
					StrutsTreeNodeManager.addNode(nodeNew.getId(), nodeNew);
					node.addChildren(nodeNew);
					queue2.offer(nodeNew);
				}
			}
		}// end while
			// ����д��DB
		ServiceManager sm = new ServiceManager();
		try {
			sm.addServices(list);
		} catch (SQLException e) {
			System.err.println(e.getMessage());
		}
		return list;
	}// end function

	
	public void testServiceIndexXml() {
		ServiceManager.importServices("D:/ServiceDefXml/index.service.xml");
		StrutsTreeNodeManager.getById(StrutsTreeNodeManager.ROOT_ID);
	}
}
