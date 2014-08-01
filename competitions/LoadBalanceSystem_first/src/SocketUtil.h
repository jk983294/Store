/***************************************************
�ļ����� SocketUtil.h
˵����	��socket�׽��ֺ����ķ�װ
���ߣ�   ����
ʱ�䣺	2013-07-16
�汾��	����
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
	static bool WinSockInit();							//Windows Socket ��ʼ��	
	static bool create_udp_socket(SOCKET &s);		    //����һ��udp socket
	static bool udp_bind(SOCKET &s, u_short port);		//udp socket s�󶨵�ָ��port��
};

#endif