package support;

import java.util.HashMap;
import java.util.Map;
/**
 * 管理Struts的树形结构显示的节点
 * @author Jersey
 * @date 2011-4-19
 */
public class StrutsTreeNodeManager {
	private static Map<String, StrutsTreeNode> nodeMap = new HashMap<String, StrutsTreeNode>();
	public static  final String ROOT_ID="1";
	public static StrutsTreeNode getById(String id) {
		return nodeMap.get(id);
	}
	public static void addNode(String id, StrutsTreeNode node){
		nodeMap.put(id, node);
	}
}
