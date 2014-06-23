#include "../include/DataBus.h"

/**************************************************	
方法名称：Sender
输入参数：s：SOCKET句柄。
*************************************************/
CTCPSender::CTCPSender(SOCKET s)
{
	s_ = s;
}

/**************************************************	
方法名称：~Sender
**************************************************/
CTCPSender::~CTCPSender()
{
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
int CTCPSender::Send(int topicID,char* buffer , int len)
{	
	int sendlen = 0;
	int tmplen = 0;
	char* tmpbuf = buffer;
	while(sendlen<len)
	{
		if((tmplen = send(s_ , tmpbuf , len-sendlen , 0)) == SOCKET_ERROR)
		{		
			int err = GetLastError();
			printf("Send Error : %d !\n" , err);		
			return -1;
		}
		sendlen+=tmplen;
		tmpbuf+=tmplen;
	}
	//printf("tcp sended %d bytes .\n",ret);	
	return len;	
}