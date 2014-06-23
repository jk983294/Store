#ifndef CTCPRECEIVER_H
#define CTCPRECEIVER_H

/**************************************************	
	类    名：Receiver
	功能说明：
	  在线程执行过程中循环读取相应端口，并且将数据封装为
	  相应的结构。如果收到原始数据，则将原始数据存储在缓存中
**************************************************/
class CTCPReceiver:public CReceiver
{
public:
	CTCPReceiver(SOCKET s );


	~CTCPReceiver();
	/*
	 *	调用该接口得到存储在buffer_中的原始数据。通过信号量sema_对读写过程
	 *	进行同步
	 */
	int Receive(TopicID& topic, char*& data , int& length);

	/*
	 *	循环读取网络端口，在信息管理组件一端,当发现是心跳信息的时候，需要
	 *	执行clearDida操作清空dida_；当发现原始数据的时候，需要将原始数据存
	 *	储在buffer_中，然后修改信号量。
	 */
	
private:

	//SOCKET句柄
	SOCKET s_;

	//存储当前的原始数据的缓存
	char tcp_header[TCP_HEADER_LEN];
};
#endif