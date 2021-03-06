/***************************************************
文件名： server_main.cpp
说明：	服务端程序的主函数
作者：   董丹
时间：	2013-08-1
版本：	复赛
***************************************************/
#include "server.h"

int main(int argc, char* argv[]){
	SocketUtil::WinSockInit();
	//参数格式 server.exe id port
	if (argc == 3)
	{
		unsigned id;
		unsigned short port;
		stringstream ss;
		ss.clear(); ss<<argv[1];	ss>>id;
		ss.clear(); ss<<argv[2];	ss>>port;
		Server server(id,port);
		server.run();
	}
	else{
		cout<<"服务器程序参数不正确"<<endl;
	}
	

	return 0;
}