/***************************************************
�ļ����� Configure.h
˵����	һϵ�в����Լ���Ϣ���͵Ķ���
���ߣ�   ����
ʱ�䣺	2013-07-16
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
const int HEARTBEAT_TIMEOUT = 10*1000;			//������ʱʱ��
const int HEARTBEAT_SEND_INTERVAL = 6*1000;    //ÿ����������������ʱ����

enum ServerCondition{
	GOOD,										//��ʼ��Ϊ���������Ǻõ�
	GOOD_HEARTBEAT_NO_REPLY_YET,               //����������֮ǰ״̬��GOOD������������û�лظ�
	BAD_HEARTBEAT_NO_REPLY_YET,				   //����������֮ǰ״̬��BAD���ôη���������û�лظ�
	BAD											//û�лظ���������Ϊ��״̬���Ժ���תѡ���ʱ�򲻻�ѡ����
};

//����Ϊ���ֺͲ���ϵͳ��Դ��صĹ��ߺ궨��
//��������Դ
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
struct Peer{
	unsigned id;
	unsigned short port;
	ServerCondition condition;
};

#endif