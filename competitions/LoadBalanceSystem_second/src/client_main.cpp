/***************************************************
文件名： client_main.cpp
说明：	客户端程序的主函数
作者：   董丹
时间：	2013-08-1
版本：	复赛
***************************************************/
#include "client.h"

int main(int argc, char* argv[]){
	SocketUtil::WinSockInit();
	//参数格式 client.exe id usr_id n
	if (argc == 4)
	{
		unsigned id,usr_id,n;
		stringstream ss;
		ss.clear();	ss<<argv[1];	ss>>id;
		ss.clear();	ss<<argv[2];	ss>>usr_id;
		ss.clear();	ss<<argv[3];	ss>>n;
		Client client(id,usr_id,n);
		client.run();
	}
	else{
		cout<<"客户端参数不正确"<<endl;
	}
	return 0;
}