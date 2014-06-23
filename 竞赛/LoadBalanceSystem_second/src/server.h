/***************************************************
�ļ����� server.h
˵����	�����������ͷ�ļ�
���ߣ�   ����
ʱ�䣺	2013-08-1
�汾��	����
***************************************************/

#ifndef SERVER_H
#define SERVER_H

#include "SocketUtil.h"
#include "Configure.h"

class Server
{
public:
	SOCKET mysocket;								//�����׽���
	unsigned short myport;							//�����˿ں�
	unsigned id;											//Ψһid������ͬ
	unsigned int RecvCorrectCount,RecvIncorrectCount,SendCount;	//�յ���ȷ���������յ��������������ͱ�����
	CThread server_recv_thread;					//���պͷ�����Ϣ���߳�
	bool isdebug;										//���Կ���
	MUTEX mutex_print;								//����Ļ��ӡ��Ϣ�Ļ�����
public:
	Server(unsigned myid,	unsigned short port);
	~Server();
	void run();																	//���������̣����ڽ����˹�����
	void run_server_recv_thread();									//���պͷ�����Ϣ���߳����г���
private:
	void print_debugInformation(string &s);					//��ӡdebug��Ϣ
	void print_debugInformation(char * info);					//��ӡdebug��Ϣ������
	void print_debugInformation_msg(string &s,t_msg &msg);						//��ӡdebug��Ϣ��msg��Ϣ
	void print_debugInformation_msgNodate(char * content,t_msg &msg);	//��ӡdebug��Ϣ�Ͳ���ʱ���ֶε�msg��Ϣ
	void print_statistics();												//��ӡͳ����Ϣ
};

#endif