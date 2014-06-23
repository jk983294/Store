package kmaru.jchord;

import java.util.ArrayList;
import java.util.List;

public class TreeNodes {
	private String state;
    private String data;
    private List<TreeNodes> children;
    public String file;
    /*private TreeAttributes attributes;
    public TreeAttributes getAttributes() {
        return attributes;
    }
    public void setAttributes(TreeAttributes attributes) {
        this.attributes = attributes;
    }*/
    public String getState() {
        return state;
    }
    public void setState(String state) {
        this.state = state;
    }
    public String getData() {
        return data;
    }
    public void setData(String data) {
        this.data = data;
    }
    public List<TreeNodes> getChildren() {
        return children;
    }
    public void setChildren(List<TreeNodes> children) {
        this.children = children;
    }
    public void addChildren(TreeNodes childnode) {
    	if(children == null)
    		children = new ArrayList<TreeNodes>();
    	children.add(childnode);
    }
    
    public TreeNodes() {
    }
    public TreeNodes(String data) {
    	this.data  =data;
    }
    public void print() {
    	if(children==null)
    		System.out.print(file+" saved in ");  
    	if(children != null){
    		//System.out.println(children.size());
    		for(int i=0;i<children.size();i++){
    			children.get(i).print();
    		}
    	}
    	System.out.print(data);
    }

}
