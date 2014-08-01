/***************************************************
文件名： SocketUtil.h
说明：	对socket套接字函数的封装
作者：   董丹
时间：	2013-07-16
版本：	初赛
***************************************************/


#ifndef SOCKETUTIL_H
#define SOCKETUTIL_H
#pragma comment(lib,"wsock32.lib")
#include <winsock.h>
#include <iostream>

using namespace std;

class SocketUtil
{	
public:
	static bool WinSockInit();							//Windows Socket 初始化	
	static bool create_udp_socket(SOCKET &s);		    //创建一个udp socket
	static bool udp_bind(SOCKET &s, u_short port);		//udp socket s绑定到指定port上
};

#endif