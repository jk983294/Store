package listeners;

import java.util.Timer;
import java.util.TimerTask;
import java.util.concurrent.Executors;

import javax.servlet.ServletContext;
import javax.servlet.ServletContextEvent;
import javax.servlet.ServletContextListener;

import support.GlobalParameter;
import support.ThreadPoolManager;
import backStage.communication.Communicator;
import backStage.communication.CommunicatorFactory;
import backStage.communication.ROIS_Server_Listener;
import backStage.worker.CheckBidDeadline;
import business.managers.ServiceManager;
/**
 * �ǳ���Ҫ��Listener����WEBӦ������ʱ���г�ʼ����̨�̵߳Ĺ���
 * @author Jersey
 * @date 2011-4-19
 */
public class StartupListener implements ServletContextListener {
	@Override
	public void contextDestroyed(ServletContextEvent sce) {
		// TODO Auto-generated method stub

	}
	/**
	 * ���WEB����������ʱ��һϵ��׼������
	 */
	@Override
	public void contextInitialized(ServletContextEvent sce) {
		sce.getServletContext().log(
				"װ�ط����б��ļ�:" + GlobalParameter.SrvIndexFilePath());
		ServiceManager.importServices(GlobalParameter.SrvIndexFilePath());
		sce.getServletContext().log("�����б�װ�����");

		sce.getServletContext().log("�����̳߳�");
		// Start a thread pool to deal with different task;
		ThreadPoolManager.setPool(Executors.newFixedThreadPool(10));

		sce.getServletContext().log("����ͨ��ģ������˿�");
		ThreadPoolManager.execute(new ROIS_Server_Listener());
		sce.getServletContext().log("����ͨ��ģ�����");

		// ������ʱ��龺���ֹ������
		this.startTimerTask(sce.getServletContext());
		
//��������Ҳȫ��Ϊ�˲������գ���
//		sce.getServletContext().log("����һ���ӳ�30��ĺ�̨�߳�ģ�⽻��ģ���յ������б��Ķ���");
//		Timer timer = new Timer(true);
//		timer.schedule(new MyTimerTask(), 10 * 1000);
	}

	/**
	 * ģ��ͨ��ģ���յ������б�
	 */
	private class MyTimerTask extends TimerTask {
		@Override
		public void run() {
			Communicator c = CommunicatorFactory.getCommunicator();
			c.recvTaskList(null);
		}
	}

	private void startTimerTask(ServletContext context) {
		context.log("������ʱ������ʱ���bidDeadline");
		// Create a Daemon timer thread
		Timer timer = new Timer(true);
		// ÿ��60����ִ������ TODO:�ǵø�Ϊ60*60*1000
		timer.schedule(new CheckBidDeadline(context), 30 * 1000, 5*60 * 1000);
		context.log("�Ѿ����������ȱ�timer.schedule(new CheckBidDeadline(context), 30 * 1000, 60 * 60 * 1000);");
	}

}
