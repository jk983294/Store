package support;

import java.util.ArrayList;
import java.util.List;
/**
 * 用于Struts树形结构的节点
 * @author Jersey
 * @date 2011-4-19
 */
public class StrutsTreeNode implements IStrutsTreeNode {
	private String name;
	private List<StrutsTreeNode> children;
	private String id;

	public StrutsTreeNode(int id,String name,StrutsTreeNode...children) {
		setName(name);
		setId(String.valueOf(id));
		this.children=new ArrayList<StrutsTreeNode>();
		for(StrutsTreeNode child:children){
			this.children.add(child);
		}
	}
	public void setId(String id) {
		this.id = id;
	}

	@Override
	public String getId() {
		return id;
	}

	public void setName(String name) {
		this.name = name;

	}

	@Override
	public String getName() {
		return name;
	}

	@Override
	public List<StrutsTreeNode> getChildren() {
		return this.children;
	}

	public boolean addChildren(StrutsTreeNode node) {
		return children.add(node);
	}

	public void setChildren(List<StrutsTreeNode> children) {
		this.children = children;
	}

}
