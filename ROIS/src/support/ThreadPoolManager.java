package support;

import java.util.concurrent.ExecutorService;
/**
 * �����־Ͷ������̳߳ع�����
 * @author Jersey
 * @date 2011-4-19
 */
public class ThreadPoolManager {
	private static ExecutorService pool;
	public static ExecutorService getPool(){
		return pool;
	}
	public static void setPool(ExecutorService pool){
		ThreadPoolManager.pool=pool;
	}
	public static void execute(Runnable command){
		getPool().execute(command);
	}
}
