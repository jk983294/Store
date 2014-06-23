package backStage.communication;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

import support.ThreadPoolManager;
/**
 * ͨѶģ��ײ����˼����˿�
 * @author Jersey
 * @date 2011-4-19
 */
public class ROIS_Server_Listener implements Runnable{
	
	private ServerSocket server_listener;
	private Socket jbpm_client;
	
	public ROIS_Server_Listener(){
		// TODO Auto-generated method stub
		server_listener = null;
		jbpm_client = null;// ����jbpm������
		
		try {
			server_listener = new ServerSocket(5001);// ����һ��ServerSocket�ڶ˿�5000�����ͻ�������
			System.out.println("��ʼ��������...");
		} catch (Exception e) {
			e.printStackTrace();
		}

	}
	
	@Override
	public void run() {
		boolean listening = true;
		while (listening)// һֱѭ������
		{
			try {
				jbpm_client = server_listener.accept();
				//ʹ���̳߳أ�����һ���߳�
				ThreadPoolManager.execute(new ROIS_Server(jbpm_client));
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}

		}


	}
}
