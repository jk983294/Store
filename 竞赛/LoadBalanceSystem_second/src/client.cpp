/***************************************************
�ļ����� client.cpp
˵����	����ͻ������Դ�ļ�
���ߣ�   ����
ʱ�䣺	2013-08-1
�汾��	����
***************************************************/

#include "client.h"

DWORD WINAPI StartClientSendThreadAddress(LPVOID param){
	Client * thread=(Client *) param;
	thread->run_client_send_thread();
	return 0;
}
DWORD WINAPI StartClientRecvThreadAddress(LPVOID param){
	Client * thread=(Client *) param;
	thread->run_client_recv_thread();
	return 0;
}
Client::Client(unsigned id,unsigned usr_id, unsigned n){
	this->id = id;	this->usr_id = usr_id;	this->n = n;
	RecvCorrectCount=RecvIncorrectCount=SendCount = 0;
	initilize(conf_path);
	if (! SocketUtil::create_udp_socket(mysocket))
		print_debugInformation("����client socket����");
	isdebug = true;
	MUTEX_INIT(mutex_sendto);
	MUTEX_INIT(mutex_print);
}
Client::~Client(){
	MUTEX_DESTROY(mutex_sendto);
	MUTEX_DESTROY(mutex_print);
	TerminateThread(client_send_thread.ThreadHandle, 0);
	TerminateThread(client_recv_thread.ThreadHandle, 0);
	closesocket(mysocket);
}
void Client::initilize(string conf_file){
	ifstream ifs(conf_file.c_str());
	string filepath = "../"+conf_file;
	if (! ifs.good())	ifs.open(filepath.c_str());
	if(ifs.good()){
		string key,value;
		stringstream ss;
		while (ifs>>key>>value)
		{
			if (key == "client_udp_port"){
				ss.clear();	ss<<value;	ss>>lb_port;
			}else if (key == "LB_id"){
				ss.clear();	ss<<value;	ss>>dst_id;
			}
		}
	}else{
		print_debugInformation("��ȡ�����ļ�ʧ��");
	}
}
void Client::run_client_send_thread(){
	Sleep(5*1000);
/*	int ret;*/
	struct sockaddr_in target;			//�Է���Ϣ	
	target.sin_family=AF_INET;    
	target.sin_addr.s_addr=inet_addr(ip.c_str());
	target.sin_port = htons(lb_port);    
	t_msg msg;
	int SEND_LEN = sizeof(t_msg),	ADDR_LEN = sizeof(target);
	
	msg.src_id = id;	msg.usr_id = usr_id;	msg.dst_id = dst_id;
	msg.msg_type = MSG_TYPE_TIME_REQUST; msg.data[0] = '\0';

	for (unsigned i=0; i < n;	++i){
		if(send_data_thread_safe(msg,SEND_LEN,target,ADDR_LEN) < 0){
			if(isdebug) perror("����ʱ�������Ĵ���: ");
		}else{
			++SendCount;
			if(isdebug)	print_debugInformation_msgNodate("����ʱ�������ĳɹ�: ",msg);
		}
		Sleep(CLIENT_SEND_INTERVAL);
	}
}
void Client::run_client_recv_thread(){
	Sleep(50);
	unsigned srcid;
	struct sockaddr_in from_;
	t_msg msg;
	stringstream ss;	string content;
	int BUFFER_LEN = sizeof(t_msg),	ADDR_LEN = sizeof(from_);
	while (true)
	{
		if(recvfrom(mysocket, (char*)&msg, BUFFER_LEN, 0, (struct sockaddr *) &from_, (int *)&ADDR_LEN) < 0){
			//sendto��ICMP����ᵼ�´˴���������winsock��bug���˴����θô���
			//������ͼ�http://support.microsoft.com/kb/263823
//			if(isdebug) perror("�Ӹ��ؾ����������մ���: ");	
		}
		else{
			if (msg.usr_id == usr_id){
				if (msg.msg_type == MSG_TYPE_TIME_REPLY){	//ʱ������ظ�����
					++RecvCorrectCount;
					if (isdebug){
						print_debugInformation_msg("�ɹ����յ�ʱ��ظ�����: ",msg);
					}
				}else{
					++RecvIncorrectCount;
					if(isdebug) print_debugInformation("�Ӹ��ؾ������յ�δ������Ϣ���͵ı���");
				}
			}else{
				++RecvIncorrectCount;
				if(isdebug) print_debugInformation("�Ӹ��ؾ������յ�Ŀ���ַ�����ҵı���");
			}
		}
	}
}
void Client::run(){
	client_recv_thread.ThreadHandle = CreateThread(NULL, 0, StartClientRecvThreadAddress, this, 0, &client_recv_thread.ThreadID);
	client_send_thread.ThreadHandle = CreateThread(NULL, 0, StartClientSendThreadAddress, this, 0, &client_send_thread.ThreadID);
	int choice;
	bool isEnd = false;
	cout<<"�ͻ��� ( id= "<<id<<" usr_id = "<<usr_id<<" ) ����..."<<endl
		<<"��1 ��debug����"<<endl
		<<"��2 �ر�debug����"<<endl
		<<"��3 ��ʾͳ������"<<endl
		<<"��4 �رտͻ��˳���"<<endl;
	while (cin>>choice)
	{		
		switch(choice){
		case 1:	isdebug = true;		break;
		case 2: isdebug = false;	break;
		case 3: print_statistics();	break;
		case 4: print_debugInformation("�رտͻ��˳���...");		isEnd = true;	break;
		default: print_debugInformation("û�и�ָ��");	break;
		}
		if (isEnd)	break;
	}
}
int Client::send_data_thread_safe(t_msg &msg,int &sendlen,struct sockaddr_in &targetaddr,int &addrlen){
	int send_success_length=0;
	MUTEX_LOCK(mutex_sendto);
	send_success_length =sendto(mysocket, (char*)&msg, sendlen, 0, (struct sockaddr *) &targetaddr, addrlen);
	MUTEX_UNLOCK(mutex_sendto);
	return send_success_length;	
}

void Client::print_debugInformation(string &s)
{
	MUTEX_LOCK(mutex_print);
	cout<<s<<endl;
	MUTEX_UNLOCK(mutex_print);

}
void Client::print_debugInformation(char * info)
{
	MUTEX_LOCK(mutex_print);
	cout<<info<<endl;
	MUTEX_UNLOCK(mutex_print);

}
void Client::print_debugInformation_msg(char * info,t_msg &msg)
{
	MUTEX_LOCK(mutex_print);
	cout<<info<<endl;
	cout<<"src_id: "<<msg.src_id<<" usr_id: "<<msg.usr_id<<" dst_id: "<<msg.dst_id
		<<" msg_type: "<<msg.msg_type<<" data: "<<msg.data<<endl;
	MUTEX_UNLOCK(mutex_print);
}
void Client::print_debugInformation_msgNodate(char * content,t_msg &msg)
{
	MUTEX_LOCK(mutex_print);
	cout<<content<<endl;
	cout<<"src_id: "<<msg.src_id<<" usr_id: "<<msg.usr_id<<" dst_id: "<<msg.dst_id
		<<" msg_type: "<<msg.msg_type<<endl;
	MUTEX_UNLOCK(mutex_print);
}
void Client::print_statistics(){
	MUTEX_LOCK(mutex_print);
	cout<<"���͵ı�����: "<<SendCount<<" �����ܱ�������"<<RecvCorrectCount+RecvIncorrectCount<<" (�յ���ȷ������: "<<RecvCorrectCount<<" �յ���������: "<<RecvIncorrectCount
		<<")"<<endl;
	MUTEX_UNLOCK(mutex_print);
}