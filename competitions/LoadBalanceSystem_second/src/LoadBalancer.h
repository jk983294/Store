/***************************************************
�ļ����� LoadBalancer.h
˵����	���ؾ��������ͷ�ļ�
���ߣ�   ����
ʱ�䣺	2013-08-1
�汾��	����
***************************************************/


#ifndef LOADBALANCER_H
#define LOADBALANCER_H

#include "Configure.h"
#include "SocketUtil.h"
#include "Logger.h"
using namespace std;


class LoadBalancer
{
public:
	SOCKET client_socket,	server_socket;									//�շ��ͻ��ˡ��������˵��׽���
	unsigned short client_udp_port,	server_udp_port;				//�����ͻ��ˡ��������˵Ķ˿�
	unsigned id;																			//Ψһid
	int server_support_count;														//֧�ַ�������
	LoadBalanceAlogrithm load_balance_algorithm;                   //���ؾ����㷨
	SessionKeepMethod session_keep_method;                         //�Ự���ֹ���
	int choosen_server;																//��ת��ѡ��ķ������±�
	vector<Peer> servers;															//��������Ϣ
	map<unsigned, struct session_keep_in_server> usrid_serverid;				//����ͻ��˵�usrid�ͷ��������id�ͻỰ����ʱ���ӳ��
	map<unsigned, struct session_keep_in_server> srcid_serverid;				//����ͻ��˵�src_id�ͷ��������id�ͻỰ����ʱ���ӳ��
	map<unsigned, struct sockaddr_in> clients_usrid_addr;						//����ͻ��˵�usrid�͵�ַ��ӳ��
	map<unsigned, unsigned>	clients_usrid_dstid;									//����ͻ��˵�usrid��id��ӳ��
	unsigned int ClientRecvCorrectCount,ClientRecvIncorrectCount,ClientSendCount;	//�ͻ��ˣ��յ���ȷ���������յ��������������ͱ�����
	unsigned int ServerRecvCorrectCount,ServerRecvIncorrectCount,ServerSendCount;	//�������ˣ��յ���ȷ���������յ��������������ͱ�����
	CThread pkt_thread;										//�շ������߳�
	CThread send_heartbeat_thread;					//��ʱ�������������߳�
	CThread detect_thread;									//���������Ƿ���ֹ����Լ��Ự�Ƿ�ʱ
	Logger *logger;												//��־
	bool isdebug;												//���Կ���
	MUTEX mutex_sendto;									//�������������ʱ���������̵߳Ļ�����
	MUTEX mutex_print;										//����Ļ��ӡ��Ϣ�Ļ�����
	MUTEX mutex_session_modify;						//�޸ĻỰ��Ŀ����
public: 
	LoadBalancer();
	~LoadBalancer();
	void initilize_socket();															//��ʼ��client_socket��server_socket		
	void initilize(string conf_file);													//��ȡ�����ļ�
	void run();																				//�����̣���Ҫ���������˹�ָ��
	void run_pkt_thread();															//�շ������߳�����
	void run_send_heartbeat_thread();										//�����������ĵ��߳�
	void run_detect_thread();														//����̵߳�����
	//��֤�ظ������ͷ���ʱ�����������̵߳ĵ����̰߳�ȫ
	int send_data_thread_safe(t_msg &msg,int &sendlen,struct sockaddr_in &targetaddr,int &addrlen);	
	void pick_server(LoadBalanceAlogrithm load_balance_algorithm);		//����ĳһ���ؾ����㷨ѡȡ������	
	void pick_server_session(t_msg &msg);												//֧�ֻỰ���ֹ���ʱ����ѡȡ�ķ�����
private: 
	void pick_server_round();							//��ת�㷨
	void pick_server_proportion();					//�������ַ����ؾ����㷨
	void pick_server_fastest();							//���������Ӧ�ĸ��ؾ����㷨
	void print_statistics();								//��ӡͳ����Ϣ														                                                 
	void printDebugInformation(string &s);    //��ӡdebug��Ϣ
	void printDebugInformation(char * info);  //��ӡdebug��Ϣ    
	void printDebugInformation_msg(string &s,t_msg &msg);					//��ӡdebug��Ϣ��msg��Ϣ
	void printDebugInformation_NoDateMsg(string &s,t_msg &msg);		//��ӡdebug��Ϣ�Ͳ���ʱ���ֶε�msg��Ϣ
	void printDebugInformation_HeartBeatMsg(string &s,t_msg &msg);		//��ӡdebug��Ϣ������������Ϣ
};


#endif