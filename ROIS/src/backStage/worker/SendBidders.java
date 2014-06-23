package backStage.worker;

import backStage.communication.Communicator;
import backStage.communication.CommunicatorFactory;
/**
 * ͨ������ͨѶģ�鷢�;�����ҵ�б�
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
		// ����ͨѶģ�鷢�;�����Ϣ
		Communicator com=CommunicatorFactory.getCommunicator();
		com.sendBiddingEntps(biddingXml);
	}

}
