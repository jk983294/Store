package kmaru.jchord;

import java.util.ArrayList;
import java.util.List;
import java.util.SortedMap;
import java.util.TreeMap;

public class Chord {

	public List<ChordNode> nodeList = new ArrayList<ChordNode>();
	private SortedMap<ChordKey, ChordNode> sortedNodeMap = new TreeMap<ChordKey, ChordNode>();
	Object[] sortedKeyArray;

	public void createNode(String nodeId,int index) throws ChordException {
		ChordNode node = new ChordNode(nodeId,index);
		nodeList.add(node);
		
		if (getSortedNodeMap().get(node.getNodeKey()) != null ) {
			throw new ChordException("Duphlicated Key: " + node);
		}
		
		getSortedNodeMap().put(node.getNodeKey(), node);
	}

	public ChordNode getNode(int i) {
		return (ChordNode) nodeList.get(i);
	}

	public ChordNode getSortedNode(int i) {
		if (sortedKeyArray == null) {
			sortedKeyArray = getSortedNodeMap().keySet().toArray();
		}
		return (ChordNode) getSortedNodeMap().get(sortedKeyArray[i]);
	}

	public void setSortedNodeMap(SortedMap<ChordKey, ChordNode> sortedNodeMap) {
		this.sortedNodeMap = sortedNodeMap;
	}

	public SortedMap<ChordKey, ChordNode> getSortedNodeMap() {
		return sortedNodeMap;
	}
	
	public void Handle_Rule(String rule){
		char ext=rule.charAt( rule.indexOf("-")-1);
	    int rule_index = rule.indexOf("-")-2;
	    ChordNode a = getSortedNode(0);
	    ChordKey ck =  new ChordKey(String.valueOf(ext));    
	    ChordNode node = a.findSuccessor(ck);
	    if(node ==null )
	    	System.out.println("no key match");	    
	    TreeNodes father = node.treenode;
	    father.setData(String.valueOf(ext));
	    for(int i =rule_index;i>=0;i-- ){
	    	TreeNodes childnode = new TreeNodes(String.valueOf(rule.charAt(i)));    	
	    	if(i == 0){
	    		childnode.file = rule.substring(rule.indexOf(">")+1);
	    	}
	    	father.addChildren(childnode);
	    	father = childnode;
	    }
	}
}
