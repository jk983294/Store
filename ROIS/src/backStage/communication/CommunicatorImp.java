package backStage.communication;

import support.GlobalParameter;
import support.OperationType;
import support.ThreadPoolManager;
import backStage.communication.ROIS_Client.ROIS_JBPM;
import backStage.worker.RecvBidWinner;
import backStage.worker.RecvOperation;
import backStage.worker.RecvTaskList;
/**
 * ͨѶ�ӿڵ�ʵ���࣬���õײ���ʵ��ͨѶ�еķ���ģ��
 * @see IClient
 * @author Jersey
 * @date 2011-4-19
 */
public class CommunicatorImp implements Communicator{
	@Override
	public void sendOrder(String path) {
		ROIS_Client client=IClientFactory.getClient(GlobalParameter.JbpmIP());
		client.sendMsgHead(ROIS_Client.ROIS_JBPM.SUBMIT_ORDER);
		client.sendFileToJBPM(path);
		client.close();
	}

	@Override
	public void recvTaskList(String filePath) {
		//���� ģ���յ�һ�������б��ĵ�
//		String path=new XmlDocTest().generateTaskListXml();
//	
		System.out.println("����ͨ��ģ������̳߳�����������ĵ����պ�ķ���������"+filePath);
		ThreadPoolManager.execute(new RecvTaskList(filePath));
	}

	@Override
	public void sendBiddingEntps(String path) {
		System.out.println("���ý���ͨ��ģ�鷢��BiddingEntp");
		ROIS_Client client = IClientFactory.getClient(GlobalParameter.JbpmIP());
		client.sendMsgHead(ROIS_JBPM.BID_TASK);
		client.sendFileToJBPM(path);
		client.close();
		//����bidding ��ҵ��Ϣ
		//FakeJbpm.setBiddingListXml(path);
	}

	@Override
	public void recvBidWinner(String winnerXml) {	
		ThreadPoolManager.execute(new RecvBidWinner(winnerXml));
	}

	@Override
	public void sendReport(String tid, OperationType optType) {
		System.out.println("����ͨ��ģ��ı������������");
		ROIS_Client client = IClientFactory.getClient(GlobalParameter.JbpmIP());
		client.sendMsgHead(ROIS_JBPM.TASK_STATUS);
		client.sendMsgContent(tid);
		client.sendMsgContent(optType.toString());
		client.close();
	}

	@Override
	public void recvOperation() {
		//TODO:��ʵ�� ���չ�����������ָ��
		ThreadPoolManager.execute(new RecvOperation());
		
	}


}
