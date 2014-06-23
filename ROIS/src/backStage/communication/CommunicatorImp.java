package backStage.communication;

import support.GlobalParameter;
import support.OperationType;
import support.ThreadPoolManager;
import backStage.communication.ROIS_Client.ROIS_JBPM;
import backStage.worker.RecvBidWinner;
import backStage.worker.RecvOperation;
import backStage.worker.RecvTaskList;
/**
 * 通讯接口的实现类，调用底层类实现通讯中的发送模块
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
		//测试 模拟收到一个任务列表文档
//		String path=new XmlDocTest().generateTaskListXml();
//	
		System.out.println("交互通信模块调用线程池完成任务项文档接收后的发布工作："+filePath);
		ThreadPoolManager.execute(new RecvTaskList(filePath));
	}

	@Override
	public void sendBiddingEntps(String path) {
		System.out.println("调用交互通信模块发送BiddingEntp");
		ROIS_Client client = IClientFactory.getClient(GlobalParameter.JbpmIP());
		client.sendMsgHead(ROIS_JBPM.BID_TASK);
		client.sendFileToJBPM(path);
		client.close();
		//发送bidding 企业信息
		//FakeJbpm.setBiddingListXml(path);
	}

	@Override
	public void recvBidWinner(String winnerXml) {	
		ThreadPoolManager.execute(new RecvBidWinner(winnerXml));
	}

	@Override
	public void sendReport(String tid, OperationType optType) {
		System.out.println("交互通信模块的报告操作被调用");
		ROIS_Client client = IClientFactory.getClient(GlobalParameter.JbpmIP());
		client.sendMsgHead(ROIS_JBPM.TASK_STATUS);
		client.sendMsgContent(tid);
		client.sendMsgContent(optType.toString());
		client.close();
	}

	@Override
	public void recvOperation() {
		//TODO:待实现 接收工作流机来的指令
		ThreadPoolManager.execute(new RecvOperation());
		
	}


}
