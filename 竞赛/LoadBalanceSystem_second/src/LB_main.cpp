/***************************************************
文件名： LB_main.cpp
说明：	运行负载均衡端程序的主函数
作者：   董丹
时间：	2013-08-1
版本：	复赛
***************************************************/


#include "LoadBalancer.h"

int main(){
	SocketUtil::WinSockInit();
	LoadBalancer lb;
	lb.run();
	return 0;
}