package backStage.communication;
/**
 * ͨѶģ�鹤��������Communicator�ӿں���ʵ��
 * @see Communicator
 * @author Jersey
 * @date 2011-4-19
 */
public class CommunicatorFactory {
	/**
	 * ��ȡһ��communicator��ʵ����
	 * @see CommunicatorImp
	 * @return
	 */
	public static Communicator getCommunicator() {
		return new CommunicatorImp();
	}
}
