package backStage.communication;
/**
 * 通讯模块工厂，关联Communicator接口和其实现
 * @see Communicator
 * @author Jersey
 * @date 2011-4-19
 */
public class CommunicatorFactory {
	/**
	 * 获取一个communicator的实现类
	 * @see CommunicatorImp
	 * @return
	 */
	public static Communicator getCommunicator() {
		return new CommunicatorImp();
	}
}
