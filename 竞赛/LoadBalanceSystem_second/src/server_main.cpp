/***************************************************
�ļ����� server_main.cpp
˵����	����˳����������
���ߣ�   ����
ʱ�䣺	2013-08-1
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
		Server server(id,port);
		server.run();
	}
	else{
		cout<<"�����������������ȷ"<<endl;
	}
	

	return 0;
}