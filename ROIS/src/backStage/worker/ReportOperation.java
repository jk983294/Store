package backStage.worker;

import support.OperationType;
import backStage.communication.Communicator;
import backStage.communication.CommunicatorFactory;
/**
 * ����ͨѶģ�鱨����ҵ������Ĳ���
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
		//���÷���ģ�鷢�ͱ���
		Communicator com= CommunicatorFactory.getCommunicator();
		com.sendReport(tid, type);
		
	}
}
