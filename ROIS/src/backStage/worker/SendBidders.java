package backStage.worker;

import backStage.communication.Communicator;
import backStage.communication.CommunicatorFactory;
/**
 * 通过调用通讯模块发送竞标企业列表
 * @see Communicator
 * @author Jersey
 * @date 2011-4-19
 */
public class SendBidders implements Runnable {
	private String biddingXml;

	public SendBidders(String xmlPath) {
		this.biddingXml = xmlPath;
	}

	@Override
	public void run() {
		// 调用通讯模块发送竞标信息
		Communicator com=CommunicatorFactory.getCommunicator();
		com.sendBiddingEntps(biddingXml);
	}

}
