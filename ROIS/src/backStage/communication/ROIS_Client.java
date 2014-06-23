package backStage.communication;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;

import backStage.communication.ROIS_Server.JBPM_ROIS;

/**
 * 通讯模块底层客户端的实现
 * @author zhd
 * @date 2011-4-19
 */
public class ROIS_Client implements IClient{
	
	// 协议头
	// 平台->引擎
	public enum ROIS_JBPM {
		SUBMIT_ORDER, BID_TASK, TASK_STATUS, RECV_OK, SEND_OK
	};



	private Socket client;
	private BufferedReader in;// 读
	private PrintWriter out;// 写

	// 主动与JBPM建立连接
	public ROIS_Client(String jbpm_ip) {
		try {
			// client = new Socket(InetAddress.getLocalHost(), 5000);
			client = new Socket(InetAddress.getByName(jbpm_ip), 5000);
			// 发送socket消息，设置消息编码格式为UTF-8
			out = new PrintWriter(new OutputStreamWriter(
					client.getOutputStream(), "UTF-8"));
			// 读取socket消息
			in = new BufferedReader(new InputStreamReader(
					client.getInputStream(), "UTF-8"));

		} catch (UnknownHostException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	// 接收ROIS_Server建立的socket连接
	public ROIS_Client(Socket client) {
		this.client = client;
		// 发送socket消息
		try {
			// 发送socket消息
			out = new PrintWriter(new OutputStreamWriter(
					client.getOutputStream(), "UTF-8"));
			// 读取socket消息
			in = new BufferedReader(new InputStreamReader(
					client.getInputStream(), "UTF-8"));
		} catch (UnknownHostException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	// 发送消息头
	@Override
	public void sendMsgHead(ROIS_JBPM head) {
		out.println(head.toString());
		out.flush();
	}

	// 发送消息内容
	@Override
	public void sendMsgContent(String msg) {
		out.println(msg);
		out.flush();
	}

	// 发送文件
	@Override
	public void sendFileToJBPM( String filePath) {
		File file = new File(filePath);// 待发送的文件
		String fileName = file.getName();// 待发送的文件名
	
		// @2:发送文件名给对方
		out.println(fileName);
		out.flush();

		System.out.println(fileName);
		// @3:发送文件

		// 由文件构造BufferedReader对象，用于接收待发送的文件
		BufferedReader fin;
		InputStreamReader sr;// 这个类很重要，提供了读取文件的编码方式，统一选用UTF-8
		try {
			sr = new InputStreamReader(new FileInputStream(file), "UTF-8");
			fin = new BufferedReader(sr);
			String line;
			while ((line = fin.readLine()) != null) {
				// xml文件内容
				out.println(line);// 发送到对方
				out.flush();// 刷新
				System.out.println(line);
			}

			out.println(ROIS_JBPM.SEND_OK.toString());// 文件发送完毕，通知服务器断开连接
			out.flush();

			String respond = in.readLine();
			if (JBPM_ROIS.valueOf(respond) == JBPM_ROIS.RECV_OK) {
				fin.close();
				in.close();
				out.close();

				client.close();
			}
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	@Override
	public void close(){
		if (client != null){
			try {
				client.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}

}
