package backStage.communication;


/**
 * ͨѶģ����Ϊ�ͻ��˵ĵײ�ʵ�ֽӿ�
 * @author zhd
 * @date 2011-4-19
 */
public interface IClient {
	/**
	 * ������Ϣͷ
	 * @param head
	 */
	public void sendMsgHead(ROIS_Client.ROIS_JBPM head);
	/**
	 * ������Ϣ����
	 * @param msg
	 */
	public void sendMsgContent(String msg);
	/**
	 * ��ROIS��JBPM�����ļ�
	 * @param filePath
	 */
	public void sendFileToJBPM(String filePath);
	/**
	 * �ر�����
	 */
	public void close();

}
