/***************************************************
�ļ����� client.h
˵����	����ͻ������ͷ�ļ�
���ߣ�   ����
ʱ�䣺	2013-07-16
�汾��	����
***************************************************/
#ifndef CLIENT_H
#define CLIENT_H

#include "SocketUtil.h"
#include "Configure.h"

class Client
{
public:
	SOCKET mysocket;										//���ͼ���socket
	unsigned short lb_port;					//���ؾ���port �Լ�����port��
	unsigned id	,	usr_id	,	dst_id;					//id������ͬ��usr_id������ͬ	
	unsigned n;													//���������Ĵ���
	unsigned int RecvCorrectCount,RecvIncorrectCount,SendCount;	//�յ���ȷ���������յ��������������ͱ�����
	CThread client_send_thread,	client_recv_thread;	  //����ʱ�������ĺͽ���ʱ��Ӧ���ĵ��߳�
	bool isdebug;    //���Կ���
	MUTEX mutex_sendto;     //�����̷߳�����Ϣ�Ļ�����
	MUTEX mutex_print;      //����Ļ��ӡ��Ϣ�Ļ�����
public:
	Client(unsigned id,unsigned usr_id, unsigned n);
	~Client();
	void initilize(string conf_file);
	void run();                     //�����̳��򣬽����˹�����
	void run_client_send_thread();  //����ʱ�������ĵ��߳�����
	void run_client_recv_thread();  //����ʱ��Ӧ���ĵ��߳�����
	//��֤�ظ������ͷ���ʱ�����������̵߳ĵ����̰߳�ȫ
	int send_data_thread_safe(t_msg &msg,int &sendlen,struct sockaddr_in &targetaddr,int &addrlen);	
private:
	void print_debugInformation(string &s);   //��ӡdebug��Ϣ
	void print_debugInformation(char * info); //��ӡdebug��Ϣ
	void print_debugInformation_msg(char * content,t_msg &msg);       //��ӡdebug��Ϣ��msg��Ϣ
	void print_debugInformation_msgNodate(char * content,t_msg &msg); //��ӡdebug��Ϣ�Ͳ���ʱ���ֶε�msg��Ϣ
	void print_statistics(); //��ӡͳ����Ϣ
	
};




#endif