/***************************************************
�ļ����� Configure.h
˵����	һϵ�в����Լ���Ϣ���͵Ķ���
���ߣ�   ����
ʱ�䣺	2013-08-1
�汾��	����
***************************************************/
#ifndef CONFIGURE_H
#define CONFIGURE_H

#pragma comment(lib,"wsock32.lib")
#include <winsock.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <windows.h>
#include <time.h>
#include <errno.h>
#include "timeUtil.h"
using namespace std;

typedef struct{
	/* ��Ϣ�ķ��ͽ�����˭������˭��id */
	unsigned src_id;
	/* ��Ϣ�Ľ��ս�����˭������˭��id */
	unsigned dst_id;
	/* ���͡�ʱ��������Ϣʱ��д��
	�ظ���ʱ��Ӧ����Ϣʱ����ֵҪ��������Ϣ����һ�¡� */
	unsigned usr_id;
	/* ��Ϣ���ͣ�0, ʱ������1, ʱ���Ӧ��2, ��������3, ����Ӧ�� */
	unsigned msg_type;
	/* ����˻ظ���ʱ��Ӧ����Ϣʱ��
	��data�����뵱ǰʱ����ַ�������ʽ�硰2013-06-20 13:56:28������  */
	char data[32];
} t_msg;
const unsigned MSG_TYPE_TIME_REQUST = 0;			//ʱ������
const unsigned MSG_TYPE_TIME_REPLY = 1;				//ʱ���Ӧ
const unsigned MSG_TYPE_HEART_REQUST = 2;		//��������
const unsigned MSG_TYPE_HEART_REPLY = 3;			//����Ӧ��


const string conf_path = "configuration.txt";
const string log_path = "log.txt";
const string ip = "127.0.0.1";
const int CLIENT_SEND_INTERVAL = 5*1000;		//�ͻ��˷���ʱ�������ļ��
const int HEARTBEAT_SEND_INTERVAL = 0.5*1000;    //ÿ����������������ʱ����

enum ServerCondition{
	GOOD,										//���������Ǻõ�
	BAD											//4��û�лظ���������Ϊ���������������Ժ�ѡ���ʱ�򲻻�ѡ����
};

enum LoadBalanceAlogrithm{                  
	DistributedInProportion,					//�������ַ��ĸ��ؾ����㷨
	DistributedInFastest,							//���������Ӧ�ĸ��ؾ����㷨
	round_alogrithm								//�����е���ת�㷨
};

enum SessionKeepMethod{
	SrcIdBased,											//����src_id�ĻỰ����
	UsrIdBased,											//����usr_id�ĻỰ����
	invalid_session_keep_method               //��Ч�ĻỰ���ֹ��ܣ������ûỰ���ֹ���
};

//����Ϊ�뻥������Դ��صĹ��ߺ궨��
#define MUTEX HANDLE
#define MUTEX_LOCK(mutex_sendto) WaitForSingleObject(mutex_sendto, INFINITE)
#define MUTEX_UNLOCK(mutex_sendto) ReleaseMutex(mutex_sendto)
#define MUTEX_INIT(mutex_sendto) mutex_sendto = CreateMutex(0, FALSE, 0)
#define MUTEX_DESTROY(mutex_sendto) CloseHandle(mutex_sendto)
//�߳���Դ
#define THREADID DWORD
struct CThread{
	THREADID ThreadID;
	HANDLE ThreadHandle;
};
//�������ṹ��
typedef struct Peer{
	unsigned id;												//id
	unsigned short port;									//�˿ں�
	ServerCondition condition;						//������״̬
	unsigned load_weight;								//����������Ȩ��
	short send_heartbeat_no_accept_times;    //������δ��Ӧ���������ĵĴ���
	long reply_time;										//��������Ӧʱ��
}Peer;

typedef struct session_keep_in_server{
	long time;
	unsigned server_id;
}session_keep_in_server;

#endif