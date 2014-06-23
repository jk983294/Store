package test;

import support.ThreadPoolManager;

import backStage.communication.FakeJbpm;
import backStage.worker.RecvBidWinner;

import com.opensymphony.xwork2.ActionSupport;
/**
 * TEST ONLY
 * @author Jersey
 *
 */
public class CommunicatorAction extends ActionSupport{

	private static final long serialVersionUID = -4789325941085420954L;

	public String recvBidWinner(){
//		FakeJbpm.parseBiddingList()
//		ThreadPoolManager.execute(new RecvBidWinner());
		return NONE;
	}
	
	
}
