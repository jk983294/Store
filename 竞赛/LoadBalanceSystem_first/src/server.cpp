/***************************************************
�ļ����� server.cpp
˵����	�����������Դ�ļ�
���ߣ�   ����
ʱ�䣺	2013-07-16
�汾��	����
***************************************************/
#include "server.h"


DWORD WINAPI StartServerRecvThreadAddress(LPVOID param){
	Server * thread=(Server *) param;
	thread->run_server_recv_thread();
	return 0;
}
Server::Server(unsigned myid,	unsigned short port){
	id = myid;		myport = port;
	RecvCorrectCount=RecvIncorrectCount=SendCount = 0;
	SocketUtil::create_udp_socket(mysocket);
	if(! SocketUtil::udp_bind(mysocket,myport))
		print_debugInformation("server socket�󶨴���");
	isdebug = true;
	MUTEX_INIT(mutex_print);
}
Server::~Server()
{
	TerminateThread(server_recv_thread.ThreadHandle, 0);
	closesocket(mysocket);
	MUTEX_DESTROY(mutex_print);
}
void Server::run_server_recv_thread(){
	Sleep(10);
	unsigned tmpid,srcid;
	struct sockaddr_in from_;
	t_msg msg;
	int BUFFER_LEN = sizeof(t_msg),	ADDR_LEN = sizeof(from_);
	stringstream ss;	string content;
	while (true)
	{
		if(recvfrom(mysocket, (char*)&msg, BUFFER_LEN, 0, (struct sockaddr *) &from_, (int *)&ADDR_LEN) < 0){
			if(isdebug) perror("�Ӹ��ؾ����������մ���: ");
		}
		else{
			if (msg.dst_id == id)
			{
				if (msg.msg_type == MSG_TYPE_TIME_REQUST){	//ʱ��������
					if(isdebug){	
		
						print_debugInformation_msgNodate("�ɹ����յ�ʱ��������: ",msg);
					}

					++RecvCorrectCount;			
					strcpy((char *)(msg.data),timeUtil::currentTime().c_str());

					tmpid = msg.dst_id;		msg.dst_id = msg.src_id;	msg.src_id = tmpid; msg.msg_type = MSG_TYPE_TIME_REPLY;
					if(sendto(mysocket, (char*)&msg, BUFFER_LEN, 0, (struct sockaddr *) &from_, ADDR_LEN) < 0){
						ss<<"����ʱ��ظ����ĸ��ͻ��� ( usr_id = "<<msg.usr_id<<" ) ���� ";	content = ss.str();	ss.str(""); 
						if(isdebug) perror(content.c_str());
					}
					else{
						++SendCount;
						if(isdebug){ 
							
							ss<<"�ظ��ͻ��� ( usr_id = "<<msg.usr_id<<" ) ��Ӧ��ʱ�������ĳɹ�: ";
							content = ss.str();
							ss.str("");
							print_debugInformation_msg(content,msg);
						}
						
					}
				}
				else if (msg.msg_type == MSG_TYPE_HEART_REQUST){	//��������
					if(isdebug){	
						
						print_debugInformation("�ɹ����յ�����������!");
						//printNoData(msg);
					}
					++RecvCorrectCount;	
					srcid = msg.src_id; msg.dst_id = msg.src_id; msg.src_id = id; msg.msg_type = MSG_TYPE_HEART_REPLY;
					if(sendto(mysocket, (char*)&msg, BUFFER_LEN, 0, (struct sockaddr *) &from_, ADDR_LEN) < 0){
						ss<<"���������ظ����ı��ĸ����ؾ����� "<<srcid<<" ����  ";	content = ss.str();	ss.str(""); 
						if(isdebug) perror(content.c_str());
					}
					else{	//��ȷ������������
						++SendCount;		
						ss<<"���������ظ����ı��ĸ����ؾ����� "<<srcid<<" �ɹ�  ";	content = ss.str();	ss.str(""); 
						if(isdebug)	print_debugInformation(content);	
					}
				}
				else{	//δ������Ϣ���ͱ��Ĵ���
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
void Server::run(){
	server_recv_thread.ThreadHandle = CreateThread(NULL, 0, StartServerRecvThreadAddress, this, 0, &server_recv_thread.ThreadID);
	int choice;
	bool isEnd = false;
	cout<<"������ (id = "<<id<<" ) ����..."<<endl
		<<"��1 ��debug����"<<endl
		<<"��2 �ر�debug����"<<endl
		<<"��3 ��ʾͳ������"<<endl
		<<"��4 �رշ���������"<<endl;
	while (cin>>choice)
	{		
		switch(choice){
		case 1:	isdebug = true;		break;
		case 2: isdebug = false;	break;
		case 3: print_statistics();	break;
		case 4: print_debugInformation("�رշ���������...");		isEnd = true;	break;
		default: print_debugInformation("û�и�ָ��");	break;
		}
		if (isEnd)	break;
	}
}

void Server::print_statistics(){
	MUTEX_LOCK(mutex_print);
	int totalRecvCount = RecvCorrectCount+RecvIncorrectCount;
	cout<<"�����ܱ�������"<<totalRecvCount<<" (�յ���ȷ������: "<<RecvCorrectCount<<" �յ���������: "<<RecvIncorrectCount
		<<")"<<" Ӧ������: "<<SendCount<<endl;
	MUTEX_UNLOCK(mutex_print);
}
void Server::print_debugInformation(string &s)
{
	MUTEX_LOCK(mutex_print);
	cout<<s<<endl;
	MUTEX_UNLOCK(mutex_print);

}
void Server::print_debugInformation(char * info)
{
	MUTEX_LOCK(mutex_print);
	cout<<info<<endl;
	MUTEX_UNLOCK(mutex_print);

}
void Server::print_debugInformation_msg(string &s,t_msg &msg)
{
	MUTEX_LOCK(mutex_print);
	cout<<s<<endl;
	cout<<"src_id: "<<msg.src_id<<" usr_id: "<<msg.usr_id<<" dst_id: "<<msg.dst_id
		<<" msg_type: "<<msg.msg_type<<" data: "<<msg.data<<endl;
	MUTEX_UNLOCK(mutex_print);

}
void Server::print_debugInformation_msgNodate(char * content,t_msg &msg)
{
	MUTEX_LOCK(mutex_print);
	cout<<content<<endl;
	cout<<"src_id: "<<msg.src_id<<" usr_id: "<<msg.usr_id<<" dst_id: "<<msg.dst_id
		<<" msg_type: "<<msg.msg_type<<endl;
	MUTEX_UNLOCK(mutex_print);

}