/***************************************************
�ļ����� LB_main.cpp
˵����	���и��ؾ���˳����������
���ߣ�   ����
ʱ�䣺	2013-07-16
�汾��	����
***************************************************/


#include "LoadBalancer.h"

int main(){
	SocketUtil::WinSockInit();
	LoadBalancer lb;
	lb.run();
	return 0;
}