#ifndef CRECEIVER_H
#define CRECEIVER_H

/**************************************************	
	类    名：Receiver
	功能说明：
	  在线程执行过程中循环读取相应端口，并且将数据封装为
	  相应的结构。如果收到原始数据，则将原始数据存储在缓存中
**************************************************/
class CReceiver
{
public:
	CReceiver(){}

	~CReceiver(){}
	/*
	 *	调用该接口得到存储在buffer_中的原始数据。通过信号量sema_对读写过程
	 *	进行同步
	 */
virtual	int Receive(TopicID& topic, char*& data , int& length)=0;
};
#endif