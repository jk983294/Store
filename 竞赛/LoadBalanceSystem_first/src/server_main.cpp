/***************************************************
�ļ����� server_main.cpp
˵����	����˳����������
���ߣ�   ����
ʱ�䣺	2013-07-16
�汾��	����
***************************************************/
#include "server.h"

int main(int argc, char* argv[]){
	SocketUtil::WinSockInit();
	//������ʽ server.exe id port
	if (argc == 3)
	{
		unsigned id;
		unsigned short port;
		stringstream ss;
		ss.clear(); ss<<argv[1];	ss>>id;
		ss.clear(); ss<<argv[2];	ss>>port;
/*		cout<<id<<" "<<port<<endl;*/
		Server server(id,port);
		server.run();
	}
	else{
		cout<<"�����������������ȷ"<<endl;
		//for debug
// 		Server server(2,11102);
// 		server.run();
	}
	

	return 0;
}