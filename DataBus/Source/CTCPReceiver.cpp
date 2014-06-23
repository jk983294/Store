#include "../include/DataBus.h"
/**************************************************	
方法名称：Receiver
输入参数：
s：SOCKET句柄。
channelpeer：通道管理对象。
输出参数：无。
返回值： 无。
函数说明：此函数完成构造函数操作。
**************************************************/
CTCPReceiver::CTCPReceiver(SOCKET s )

{
	s_ = s;
	//MUTEX_INIT(recvLock_);	
	//tcp_header= new char[TCP_HEADER_LEN];
}

/**************************************************	
方法名称：~Receiver
**************************************************/
CTCPReceiver::~CTCPReceiver()
{
	closesocket(s_);

	Sleep(30);

//	MUTEX_UNLOCK(recvLock_);
	//MUTEX_DESTROY(recvLock_);
}
/**************************************************	
方法名称：receive
输入参数：无。
输出参数：buffer：存储当前的原始数据的缓存。
返回值： 原始数据的长度。
函数说明：此函数得到存储在buffer_中的原始数据。

  正常情况下返回得到的原始数据的长度，而buffer被指向原始数据的缓存
  如果在接收的过程中出现网络异常则返回-1，buffer置0
  buffer的内容创建于Receiver::run()， 通过Receiver::receive() 之后的调用传递给InfoList::insertData中的InfoNode::setRawData(rawdata , len), 在下一次接收原始数据时, InfoNode::setRawData(rawdata , len)会将本次的buffer的内容释放

循环读取网络端口，将得到的数据进行存储然后通过信号量等待读取
TopicID& flag: 0 read tcp header  1 :read tcp data
**************************************************/
int CTCPReceiver::Receive(TopicID& flag, char*& data , int& length)
{
	int ret =0;	
	if (flag == 0)
	{
		int recvlen = 0;
		int tmplen = 0;
		char* tmpbuf = tcp_header;
		while(recvlen<TCP_HEADER_LEN)
		{
			if((tmplen = recv(s_ , tmpbuf , TCP_HEADER_LEN-recvlen , 0)) == SOCKET_ERROR)
			{
				int err = GetLastError();
				printf("Recv Error : %d !\n" , err);
				return -1;
			}
			recvlen+=tmplen;
			tmpbuf+=tmplen;
		}
		memcpy(data , tcp_header , TCP_HEADER_LEN);
		return TCP_HEADER_LEN;
	}
	else if (flag == 1)
	{
		data = (char*)malloc(length);
		char* tmpbuf = data;
		memset(data , 0 , length);
		int recvlen = 0;
		int tmplen = 0;
		while(recvlen<length)
		{
			if((tmplen = recv(s_ , tmpbuf , length - recvlen , 0)) == SOCKET_ERROR)
			{
				int err = GetLastError();
				printf("Recv Error : %d !\n" , err);
				return -1;
			}
			recvlen+=tmplen;
			tmpbuf+=tmplen;
		}
		return length;
	}
	return -1;
		
}
