package backStage.communication;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

import support.ThreadPoolManager;
/**
 * 通讯模块底层服务端监听端口
 * @author Jersey
 * @date 2011-4-19
 */
public class ROIS_Server_Listener implements Runnable{
	
	private ServerSocket server_listener;
	private Socket jbpm_client;
	
	public ROIS_Server_Listener(){
		// TODO Auto-generated method stub
		server_listener = null;
		jbpm_client = null;// 来自jbpm的连接
		
		try {
			server_listener = new ServerSocket(5001);// 创建一个ServerSocket在端口5000监听客户端请求
			System.out.println("开始监听连接...");
		} catch (Exception e) {
			e.printStackTrace();
		}

	}
	
	@Override
	public void run() {
		boolean listening = true;
		while (listening)// 一直循环监听
		{
			try {
				jbpm_client = server_listener.accept();
				//使用线程池，启动一个线程
				ThreadPoolManager.execute(new ROIS_Server(jbpm_client));
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}

		}


	}
}
