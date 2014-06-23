/***************************************************
文件名： SocketUtil.cpp
说明：	对socket套接字封装函数的定义
作者：   董丹
时间：	2013-07-16
版本：	初赛
***************************************************/


#include "SocketUtil.h"


bool SocketUtil::WinSockInit(){
	WSADATA ws;
	if ( WSAStartup(MAKEWORD(2,2), &ws) != 0 ){
		return false;
	}
	return true;
}
bool SocketUtil::create_udp_socket(SOCKET &s){   
	if ((s=socket(AF_INET,SOCK_DGRAM,0))==0)     
	{    
		perror("udp socket 初始化错误");   
		return false;    
	}   
	return true;
}
bool SocketUtil::udp_bind(SOCKET &s, u_short port){
	struct sockaddr_in addr;   
	addr.sin_family=AF_INET;    
	addr.sin_addr.s_addr=htonl(INADDR_ANY);
	addr.sin_port = htons(port);        
	if (bind(s,(struct sockaddr *) &addr,sizeof(addr)) < 0)    
	{    
		perror("udp 端口绑定错误");    
		return false;    
	}    
	return true;
}