package backStage.communication;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.io.Writer;
import java.net.Socket;
import java.net.UnknownHostException;

import support.GlobalParameter;
import backStage.communication.ROIS_Client.ROIS_JBPM;
/**
 * 通讯模块底层服务端，主管接收文件和消息
 * @author zhd
 * @date 2011-4-19
 */
public class ROIS_Server implements Runnable {
	// 协议头

	// 引擎->平台
	public enum JBPM_ROIS {
		PUBLISH_TASK, BID_RESULT, RECV_OK, SEND_OK
	};

	private Socket jbpm_client;
	private BufferedReader in;// 读
	private PrintWriter out;// 写

	public ROIS_Server(Socket client) {
		jbpm_client = client;
		try {
			// 发送socket消息，设置消息编码格式为UTF-8
			out = new PrintWriter(new OutputStreamWriter(
					jbpm_client.getOutputStream(), "UTF-8"));
			// 读取socket消息
			in = new BufferedReader(new InputStreamReader(
					jbpm_client.getInputStream(), "UTF-8"));

		} catch (UnknownHostException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}

	}

	// 接收文件
	public void recvFileFromJBPM(JBPM_ROIS type) {
		File file = null;
		try {
			// @1:接收文件名称
			String fileName = in.readLine();

			file = new File(GlobalParameter.RecvFileDir()+ fileName);
			Writer writer=new OutputStreamWriter(new FileOutputStream (file),"utf-8");
			BufferedWriter bw = new BufferedWriter(writer);

			// @2:接收文件内容
			String fileContent;
			while (true) {
				fileContent = in.readLine();
				if (fileContent.equals(ROIS_JBPM.SEND_OK.toString())) {
					out.println(JBPM_ROIS.RECV_OK.toString());
					out.flush();
					System.out.println("文件传送结束");
					break;
				}

				bw.write(fileContent, 0, fileContent.length());// 写入文件
				bw.write("\r\n");// 换行
				bw.flush();
			}

			bw.close();
			writer.close();
		} catch (Exception e) {
			e.printStackTrace();
		}

		// 通知处理收到的文件
		switch (type) {
		case PUBLISH_TASK:
			CommunicatorFactory.getCommunicator().recvTaskList(
					file.getAbsolutePath());
			break;
		case BID_RESULT:
			CommunicatorFactory.getCommunicator().recvBidWinner(file.getAbsolutePath());
			break;
		default:
			break;
		}

	}

	@Override
	public void run() {
		// 接收新的连接，生成新的线程处理连接！

		// 开始任务
		String msgHead = null;
		try {
			msgHead = in.readLine();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		switch (JBPM_ROIS.valueOf(msgHead)) {
		case PUBLISH_TASK:
			recvFileFromJBPM(JBPM_ROIS.PUBLISH_TASK);
			break;
		case BID_RESULT:
			recvFileFromJBPM(JBPM_ROIS.BID_RESULT);
			break;
		default:
			break;
		}
	}
}
