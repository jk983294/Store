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
CUDPReceiver::CUDPReceiver(SOCKET s )
{
	s_ = s;	
	//buffer_ = 0;	
	buffer_len_ = 0;
	addrlen=sizeof(from);
	//Init_multicast();

	  
}
int CUDPReceiver::Init_multicast(){
	
	return 1;
}
/**************************************************	
方法名称：~Receiver
**************************************************/
CUDPReceiver::~CUDPReceiver()
{
	closesocket(s_);

	//buffer_ = 0;
	buffer_len_ = -1;

	Sleep(30);

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
**************************************************/
int CUDPReceiver::Receive(TopicID& topic, char*& data , int& length)
{
	int nbytes;
	    
	if ((nbytes=recvfrom(s_, buffer_, UDP_HEADER_LEN, 0, (struct sockaddr *) &from, (int *)&addrlen)) < 0)     
	{     
		printf("multicast error code :%d\n",GetLastError());
		return -1;    
	}  
	buffer_[nbytes] = 0;
	//printf("multicast data recv : '%s' from <%s>  in total %d bytes \n", buffer_, inet_ntoa(from.sin_addr),nbytes);
	//data= new char[UDP_HEADER_LEN];
	memcpy(data,buffer_,UDP_HEADER_LEN);
	return nbytes;
}
