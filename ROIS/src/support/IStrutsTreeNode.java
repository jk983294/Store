package support;

import java.util.List;
/**
 * 为动态StursTree提供支持的数据节点结构
 * @author Jersey
 * @date 2011-4-19
 */
public interface IStrutsTreeNode {
	public String getId();
	public String getName();
	public List<?> getChildren();

}
