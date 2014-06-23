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
 * 非常重要的Listener，在WEB应用启动时进行初始化后台线程的工作
 * @author Jersey
 * @date 2011-4-19
 */
public class StartupListener implements ServletContextListener {
	@Override
	public void contextDestroyed(ServletContextEvent sce) {
		// TODO Auto-generated method stub

	}
	/**
	 * 完成WEB服务器启动时的一系列准备工作
	 */
	@Override
	public void contextInitialized(ServletContextEvent sce) {
		sce.getServletContext().log(
				"装载服务列表文件:" + GlobalParameter.SrvIndexFilePath());
		ServiceManager.importServices(GlobalParameter.SrvIndexFilePath());
		sce.getServletContext().log("服务列表装载完毕");

		sce.getServletContext().log("启动线程池");
		// Start a thread pool to deal with different task;
		ThreadPoolManager.setPool(Executors.newFixedThreadPool(10));

		sce.getServletContext().log("启动通信模块监听端口");
		ThreadPoolManager.execute(new ROIS_Server_Listener());
		sce.getServletContext().log("启动通信模块完毕");

		// 启动定时检查竞标截止的任务
		this.startTimerTask(sce.getServletContext());
		
//下面三行也全是为了测试用哒！！
//		sce.getServletContext().log("启动一个延迟30秒的后台线程模拟交互模块收到任务列表后的动作");
//		Timer timer = new Timer(true);
//		timer.schedule(new MyTimerTask(), 10 * 1000);
	}

	/**
	 * 模拟通信模块收到任务列表
	 */
	private class MyTimerTask extends TimerTask {
		@Override
		public void run() {
			Communicator c = CommunicatorFactory.getCommunicator();
			c.recvTaskList(null);
		}
	}

	private void startTimerTask(ServletContext context) {
		context.log("启动定时器，定时检查bidDeadline");
		// Create a Daemon timer thread
		Timer timer = new Timer(true);
		// 每隔60分钟执行任务 TODO:记得改为60*60*1000
		timer.schedule(new CheckBidDeadline(context), 30 * 1000, 5*60 * 1000);
		context.log("已经添加任务调度表timer.schedule(new CheckBidDeadline(context), 30 * 1000, 60 * 60 * 1000);");
	}

}
