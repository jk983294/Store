/***************************************************
�ļ����� client_main.cpp
˵����	�ͻ��˳����������
���ߣ�   ����
ʱ�䣺	2013-07-16
�汾��	����
***************************************************/
#include "client.h"

int main(int argc, char* argv[]){
	SocketUtil::WinSockInit();
	//������ʽ client.exe id usr_id n
	if (argc == 4)
	{
		unsigned id,usr_id,n;
		stringstream ss;
		ss.clear();	ss<<argv[1];	ss>>id;
		ss.clear();	ss<<argv[2];	ss>>usr_id;
		ss.clear();	ss<<argv[3];	ss>>n;
/*		cout<<id<<" "<<usr_id<<" "<<n<<endl;*/
		Client client(id,usr_id,n);
		client.run();
	}
	else{
		cout<<"�ͻ��˲�������ȷ"<<endl;
	}
	return 0;
}