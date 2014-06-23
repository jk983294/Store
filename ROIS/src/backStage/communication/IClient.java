package backStage.communication;


/**
 * 通讯模块作为客户端的底层实现接口
 * @author zhd
 * @date 2011-4-19
 */
public interface IClient {
	/**
	 * 发送消息头
	 * @param head
	 */
	public void sendMsgHead(ROIS_Client.ROIS_JBPM head);
	/**
	 * 发送消息内容
	 * @param msg
	 */
	public void sendMsgContent(String msg);
	/**
	 * 从ROIS向JBPM发送文件
	 * @param filePath
	 */
	public void sendFileToJBPM(String filePath);
	/**
	 * 关闭连接
	 */
	public void close();

}
