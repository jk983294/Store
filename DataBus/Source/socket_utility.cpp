#include "../include/DataBus.h"

/************************************************************************
方法名称：recVarLengthData
说明：用于接收数据长度很长的网络SOCEKT数据                                                             
************************************************************************/
int CSocket_utility::recvFixedLengthData(SOCKET sclient,char* targetbuf,int length){
	int ret=0;
		if((ret = recv(sclient , targetbuf , length , 0)) == SOCKET_ERROR)
				{
					int err = GetLastError();
					printf("Recv Error : %d !\n" , err);
					return -1;
				}else{
					return ret;
				}
				

}
/************************************************************************
方法名称：recVarLengthData
说明：用于接收数据长度很长的网络SOCEKT数据，循环分次写入targetbuf                                                            
************************************************************************/
int CSocket_utility::recvVarLengthData(SOCKET sclient,char* targetbuf,int expectlen){
	int idx = 0;
	int lenLeft = expectlen;
	int ret = 0;
	do
	{
		/*期待的数据长度， 是变值*/
		ret = recv(sclient, targetbuf + idx , lenLeft, 0);
		if(ret == 0) //出现异常
		{
			printf("err  接收变长数据异常 ret=0\n");
			return false;
		}
		else if(ret == SOCKET_ERROR) //出现异常

		{
			printf("err  接收变长数据异常 ret=err\n");
			return false;
		}
		
		lenLeft-=ret;
		idx+=ret;	
	}
	while(lenLeft>0);
	targetbuf[expectlen] = '\0'; //传入的expectlen没有包含的'\0'的长度
	return expectlen;
}
/************************************************************************
sendFixedLengthData
说明：发送定长长度的数据                                                          
************************************************************************/
int CSocket_utility::sendFixedLengthData(SOCKET sclient,char* sourcebuf,int length){
	int ret=0;

		if((ret=send(sclient , sourcebuf , length , 0)) == SOCKET_ERROR)
				{

					int err = GetLastError();
					printf("Send Error : %d !\n" , err);

					return -1;
				}else{
					return ret;
				}
}
int CSocket_utility::pack_UDP_Header_packet(short type,char* source,int topic,
											char* paddr,u_short port,char flag,u_short reference){
	if(type == MSG_PUBLISH){
		u_long tmpaddr = inet_addr(paddr);
		memset(source,0,UDP_HEADER_LEN);		
		source[0] = (unsigned char)MSG_PUBLISH;
		source[1] = (unsigned char)((topic&0xff000000)>>24);
		source[2] = (unsigned char)((topic&0x00ff0000)>>16);
		source[3] = (unsigned char)((topic&0x0000ff00)>>8);
		source[4] = (unsigned char)((topic&0x000000ff));	
		source[5] = (unsigned char)((tmpaddr&0xff000000)>>24);
		source[6] = (unsigned char)((tmpaddr&0x00ff0000)>>16);
		source[7] = (unsigned char)((tmpaddr&0x0000ff00)>>8);
		source[8] = (unsigned char)((tmpaddr&0x000000ff));
		source[9] = (unsigned char)((port&0x0000ff00)>>8);
		source[10] = (unsigned char)((port&0x000000ff));
	}
	else if (type == MSG_COMPSTATE)
	{
		u_long tmpaddr = inet_addr(paddr);
		memset(source,0,UDP_HEADER_LEN);		
		source[0] = (unsigned char)MSG_COMPSTATE;
		source[1] = (unsigned char)((topic&0xff000000)>>24);
		source[2] = (unsigned char)((topic&0x00ff0000)>>16);
		source[3] = (unsigned char)((topic&0x0000ff00)>>8);
		source[4] = (unsigned char)((topic&0x000000ff));	
		source[5] = (unsigned char)((tmpaddr&0xff000000)>>24);
		source[6] = (unsigned char)((tmpaddr&0x00ff0000)>>16);
		source[7] = (unsigned char)((tmpaddr&0x0000ff00)>>8);
		source[8] = (unsigned char)((tmpaddr&0x000000ff));
		source[9] = (unsigned char)((port&0x0000ff00)>>8);
		source[10] = (unsigned char)((port&0x000000ff));
		source[11] = flag;
		source[12] = (unsigned char)((reference&0x0000ff00)>>8);
		source[13] = (unsigned char)((reference&0x000000ff));
	}
	return 1;
}
int CSocket_utility::pack_TCP_Header_packet(char* source,short type,int topic,char* paddr,
							 u_short port,u_short len)
{
	//char* tmpbuf=new char[TCP_HEADER_LEN];
	memset(source,0,TCP_HEADER_LEN);	
	if (type == MSG_ESTABLISH)
	{
		u_long tmpaddr = inet_addr(paddr);
		source[0] = (unsigned char)MSG_ESTABLISH;	
		source[1] = (unsigned char)((tmpaddr&0xff000000)>>24);
		source[2] = (unsigned char)((tmpaddr&0x00ff0000)>>16);
		source[3] = (unsigned char)((tmpaddr&0x0000ff00)>>8);
		source[4] = (unsigned char)((tmpaddr&0x000000ff));
		source[5] = (unsigned char)((port&0x0000ff00)>>8);
		source[6] = (unsigned char)((port&0x000000ff));
	}
	else if (type ==MSG_DATA_TCP)
	{
		source[0] = (unsigned char)MSG_DATA_TCP;	
		source[1] = (unsigned char)((topic&0xff000000)>>24);
		source[2] = (unsigned char)((topic&0x00ff0000)>>16);
		source[3] = (unsigned char)((topic&0x0000ff00)>>8);
		source[4] = (unsigned char)((topic&0x000000ff));
		source[5] = (unsigned char)((len&0x0000ff00)>>8);
		source[6] = (unsigned char)((len&0x000000ff));
	}
	else if ((type ==MSG_SUBSCRIBE) ||
		(type == MSG_UNSUBSCRIBE)||
		(type == MSG_UNPUBLISH))  //主题订阅报文||取消主题订阅报文格式||取消主题发布报文
	{
		if (type ==MSG_SUBSCRIBE)
			source[0] = (unsigned char)MSG_SUBSCRIBE;
		else if (type == MSG_UNSUBSCRIBE)
			source[0] = (unsigned char)MSG_UNSUBSCRIBE;
		else if (type == MSG_UNPUBLISH)
			source[0] = (unsigned char)MSG_UNPUBLISH;			
		source[1] = (unsigned char)((topic&0xff000000)>>24);
		source[2] = (unsigned char)((topic&0x00ff0000)>>16);
		source[3] = (unsigned char)((topic&0x0000ff00)>>8);
		source[4] = (unsigned char)((topic&0x000000ff));
	}
	return 1;	
}
void  CSocket_utility::unpack_TCP_Header_packet(short &type,char* tcp_packet,int& topic,
							   char*& paddr,u_short& port,u_short &len)
{
	type = (unsigned char)tcp_packet[0];
	if (type == MSG_ESTABLISH)
	{
		char tmpaddr[64]={0};
		char tempchar[32]={0};
		for (int i=0;i<4;i++)
		{
			sprintf(tempchar,"%d",(unsigned char)tcp_packet[4-i]);
			strcat(tmpaddr,tempchar);
			strcat(tmpaddr,".");
		}
		int index = strlen(tmpaddr);
		tmpaddr[index-1]='\0';
		paddr = new char[index+1];
		strncpy(paddr,tmpaddr,index);
		port = (((unsigned char)tcp_packet[5])<<8)|
		(((unsigned char)tcp_packet[6]));

		printf("tcp handshake packet recved IP: %s  port: %d\n",paddr,port);
	}
	else if (type ==MSG_DATA_TCP)
	{
		topic = (((unsigned char)tcp_packet[1])<<24)|
		(((unsigned char)tcp_packet[2])<<16)|
		(((unsigned char)tcp_packet[3])<<8)|
		(((unsigned char)tcp_packet[4]));
		len = (((unsigned char)tcp_packet[5])<<8)|
		(((unsigned char)tcp_packet[6]));
	}
	else if ((type ==MSG_SUBSCRIBE) ||
		(type == MSG_UNSUBSCRIBE)||
		(type == MSG_UNPUBLISH))  //主题订阅报文||取消主题订阅报文格式||取消主题发布报文
	{
		topic = (((unsigned char)tcp_packet[1])<<24)|
			(((unsigned char)tcp_packet[2])<<16)|
			(((unsigned char)tcp_packet[3])<<8)|
			(((unsigned char)tcp_packet[4]));
	}
}