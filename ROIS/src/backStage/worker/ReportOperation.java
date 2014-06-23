package backStage.worker;

import support.OperationType;
import backStage.communication.Communicator;
import backStage.communication.CommunicatorFactory;
/**
 * 调用通讯模块报告企业对任务的操作
 * @see Communicator
 * @author Jersey
 * @date 2011-4-19
 */
public class ReportOperation implements Runnable{
	private OperationType type;
	private String tid;
	public ReportOperation (String tid,OperationType type){
		this.tid=tid;
		this.type=type;
	}
	
	@Override
	public void run(){
		//调用发送模块发送报告
		Communicator com= CommunicatorFactory.getCommunicator();
		com.sendReport(tid, type);
		
	}
}
