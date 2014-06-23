package backStage.communication;

import support.OperationType;

/**
 * Core interface for communicating with the WFE module<br/>
 * 与工作流机模块通信的核心接口
 * 
 * @author Jersey
 * 
 */
public interface Communicator {
	/**
	 * Called by SendOrder Thread to send order.xml to WFE
	 * 
	 * @param path
	 */
	public void sendOrder(String path);

	/**
	 * Fire the recvTaskList Thread to deal the received XML file
	 */
	void recvTaskList(String filePath);

	/**
	 * Called by the checkBidDeadline Thread to send Bidding list of task to WFE
	 * 
	 * @param biddingXml
	 */
	public void sendBiddingEntps(String biddingXml);

	/**
	 * Fire the RecvBidWinner Thread to notify the euser
	 */
	public void recvBidWinner(String winnerXml);

	/**
	 * Send report to WFE, such as report task Begin or report task finished
	 * 
	 * @param tid
	 * @param reportType
	 */
	public void sendReport(String tid, OperationType opType);

	/**
	 * Receive the command from WFE,such as suspend,continue.
	 */
	public void recvOperation();

}
