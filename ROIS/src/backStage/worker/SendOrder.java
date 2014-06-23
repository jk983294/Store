package backStage.worker;

import support.GlobalParameter;
import backStage.communication.Communicator;
import backStage.communication.CommunicatorFactory;

public class SendOrder implements Runnable{
	private String orderName;
	public SendOrder(String name){
		orderName=name;
	}
	@Override
	public void run() {
		Communicator com=CommunicatorFactory.getCommunicator();
		com.sendOrder(orderName);
	}

}
