#include "../include/DataBus.h"

/**************************************************	
方法名称：Sender
输入参数：s：SOCKET句柄。
*************************************************/

CUDPSender::CUDPSender(SOCKET s)
{
	s_ = s;
	/* set up destination address */    
    memset(&dest_addr,0,sizeof(dest_addr));    
    dest_addr.sin_family=AF_INET;    
    dest_addr.sin_addr.s_addr=inet_addr(MULTICAST_GROUP);    
    dest_addr.sin_port=htons(MULTICAST_PORT);  
}

/**************************************************	
方法名称：~Sender
**************************************************/
CUDPSender::~CUDPSender()
{
	closesocket(s_);
}

/**************************************************	
方法名称：sendData
输入参数：
buffer：发送缓冲区。
len：发送数据长度。
输出参数：无。
返回值： 发送成功返回发送长度，否则返回-1。
函数说明：此函数向对端发送数据。
**************************************************/
int CUDPSender::Send(int topicID,char* buffer , int len)
{	
	//编码头信息(DELETED)
	
	//编码并发送原始数据
	int send_success_length=0;
	if ((send_success_length =sendto(s_,buffer, len, 0, 
		(struct sockaddr *) &dest_addr, sizeof(dest_addr))) < 0)     
	{    
		perror("sendto"); 
		return -1;   
	}  
	//printf("sended %d bytes\n",send_success_length);
	return send_success_length;	
}