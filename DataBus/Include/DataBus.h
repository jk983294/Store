/**********************************************
���ļ�Ϊ����������ϵͳ�е�����������ݽṹ��Ԥ��
���壬�������塢���Լ������ļ����ɴ�ȷ����������
Ŀ�Ĵ���ṹ��

1�����ļ������windowsxpϵͳ��
2��ϵͳ�и������ʵ���ļ������ø��ļ���
3��Ӧ��������ø���ĿΨһ��ͷ�ļ����Ǹ��ļ���

������2011-12-10
**********************************************/
#ifndef DATABUS_H
#define DATABUS_H

//����Ϊ���õĲ���ϵͳͷ�ļ�
#include <direct.h>
#include <errno.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <winsock.h>

//����Ϊ��Ŀʹ�õĸ��ֳ�������
const int DEFAULT_PORT = 6666; //Ĭ����ʼ����˿ں�
const int TOPIC_MAXNUM = 15; //ÿ�����������������
const int PEER_MAXNUM_FOR_EACH_TOPIC = 15; //ÿ���������Ķ�������
const int MAXPEERNUM = 15; //�ڵ�����������ӵ�CPeer������
const int MULTICAST_PORT = 7905; //������Ϣ�鲥�˿ں�    
#define MULTICAST_GROUP "228.4.5.6" //������Ϣ�鲥��ַ
enum protocolType
{
	TCP,
	UDP,
	RAPIDIO
}; //�����װ��������
const int TCP_HEADER_LEN = 8; //TCP����ͷ����
const int UDP_HEADER_LEN = 16; //�鲥��ַ�Ϸ������ĵĳ���
const int HEADER_LEN = 6; //����ͷ�ĳ���
const int DEFAULT_BUF_SIZE = 1024*8; //ԭʼ���ݵ���󳤶�

//����Ϊ���ı�ʶ
const short MSG_PUBLISH = 196; //���ⷢ������
const short MSG_COMPSTATE = 197;//�������״̬����
const short MSG_ESTABLISH = 64; //TCP�������ֱ��ġ�
const short MSG_SUBSCRIBE = 65; //���ⶩ�ı��ġ�
const short MSG_UNSUBSCRIBE = 66; //ȡ�����ⶩ�ı���
const short MSG_DATA_TCP = 67; //���ݴ��䱨��ͷ
const short MSG_UNPUBLISH = 68; //ȡ�����ⷢ������ 
const int  PACKET_INTERRUPT_CONNECT = 66; //�ж�select������

const int PEERDEADTHRESHOLD = 60; //��֤������ϢʧЧ�ĵȴ�����
const int MAX_TOPIC_SIZE=20; //���������Ŀ
const int MULTICAST_PERIOD=1; //�鲥���� ��λΪ��

//����Ϊ�����߼���ĸ��ֲ���
const int MAX_ARRAY_LENGTH = 1000; //��������󻺴����������

//����ΪӦ�÷�װ��ĸ��ֲ���
const int MAXOPERATIONUM = 8; //ÿ��������Ӧ����ඨ���������Ӧ����
const int MAXINFOHANDLERNUM = 8; //ÿ�������ඨ���������Ӫ��
const int DOMAIN_INTERVAL = 100000; //ÿ�����topicid���
const int MAX_TOPIC_SEGMENTS = 4; //ÿ�����������ֶ���
const int MAX_TOPIC_SEGLENG = 128; //ÿ�������ֶεĳ���
const int MAX_CDATA_LENGTH = 256; //ÿ�����ݵ��ֶ���
const int SUBSCRIBE_MONITOR_INFO=1; //��ض�������

#define TopicID int
#define domain_id int

//����Ϊ���ֺͲ���ϵͳ��Դ��صĹ��ߺ궨��
//��������Դ
#define MUTEX HANDLE
#define SEMA HANDLE
#define MUTEX_LOCK(mutex) WaitForSingleObject(mutex, INFINITE)
#define MUTEX_UNLOCK(mutex) ReleaseMutex(mutex)
#define MUTEX_INIT(mutex) mutex = CreateMutex(0, FALSE, 0)
#define MUTEX_DESTROY(mutex) CloseHandle(mutex)
//�ź�����Դ
#define SEMA_WAIT_TIME(sema,delay) WaitForSingleObject(sema, delay)
#define SEMA_WAIT(sema) WaitForSingleObject(sema, INFINITE)
#define SEMA_POST(sema) ReleaseSemaphore(sema, 1, NULL)
#define SEMA_TRYWAIT(sema) WaitForSingleObject(sema, 0)
#define SEMA_DESTROY(sema) CloseHandle(sema)
#define SEMA_INIT(sema, initCount, maxCount) sema = CreateSemaphore(NULL, initCount, maxCount, NULL) 
//�߳���Դ
#define THREADID DWORD

//����ΪDLL���÷�ʽ�����Լ�DLL������Դ
#ifdef DATABUS_EXPORTS   
#define DLL_API __declspec(dllexport) 
#else   
#define DLL_API __declspec(dllimport)   
#endif  

//����Ϊ��Ŀ�����е���Ԥ����
class CThread;
//����ΪӦ�÷�װ���������Ԥ����
class DLL_API CTopic;
class DLL_API CAny;
class DLL_API CData;
class DLL_API DomainParticipant;
class DLL_API DomainParticipantFactory;
class DLL_API CThread;
class DLL_API CPublisher;
class DLL_API CSubscriber;
class DLL_API CDataWriter;
class DLL_API CDataReader;
class DLL_API CDataDispatchor;
class DLL_API CDataHandler;
//����Ϊ�߼����ݲ��������Ԥ����
class CDataFlow;
class CDataArray;
class CDataCache;
class CFlowIn;
class CFlowOut;
class CPrioSetting;
class CPrioSettingTest;
class CReceivingArray;
class CSendingArray;
class CDataLogics;
//����Ϊ�����װ���������Ԥ����
class CReceiver;
class CSender;
class CAcceptor;
class CConnector;
class CDataChannel;
class CNetMonitor;
class CNetworkOperator;
class CPeer;
class CTCPAcceptor;
class CTCPConnector;
class CTCPReceiver;
class CTCPSender;
class CUDPReceiver;
class CUDPSender;
class CSocket_utility;

//����Ϊ��Ŀ���漰���������ඨ���ļ�
#include "Thread.h"
#include "CAcceptor.h"
#include "CConnector.h"
#include "CReceiver.h"
#include "CSender.h"
//����ΪӦ�÷�װ��������ඨ���ļ�
#include "Topic.h"
#include "Any.h"
#include "Data.h"
#include "Publisher.h"
#include "Subscriber.h"
#include "DomainParticipant.h"
#include "DomainParticipantFactory.h"
#include "DataWriter.h"
#include "DataReader.h"
#include "DataDispatchor.h"
#include "DataHandler.h"
//����Ϊ�߼����ݲ�������ඨ���ļ�
#include "CDataArray.h"
#include "CDataCache.h"
#include "CDataFlow.h"
#include "CFlowIn.h"
#include "CFlowOut.h"
#include "CReceivingArray.h"
#include "CSendingArray.h"
#include "DataLogics.h"
//����Ϊ�����װ��������ඨ���ļ�
#include "CDataChannel.h"
#include "NetMonitor.h"
#include "CNetworkOperator.h"
#include "CPeer.h"
#include "CTCPAcceptor.h"
#include "CTCPConnector.h"
#include "CTCPReceiver.h"
#include "CTCPSender.h"
#include "CUDPReceiver.h"
#include "CUDPSender.h"
#include "socket_utility.h"

//����Ϊ�ṩ�����ʹ�õĹ��ߺ���
DLL_API int Initialize(domain_id domain);
DLL_API CTopic CreateTopic();
DLL_API CData CreateData(CTopic topic);
DLL_API int Publish(CTopic topic);
DLL_API int UnPublish(CTopic topic);
DLL_API int UnPublish(TopicID topic);
DLL_API int Subscribe(CTopic topic);
DLL_API int Subscribe(TopicID topic);
DLL_API int UnSubscribe(CTopic topic);
DLL_API int UnSubscribe(TopicID topic);
DLL_API int Send(CData data);

#define IMPORT_DATABUS extern DLL_API CDataDispatchor* g_datadispatcher_ptr_; \
extern DLL_API CDataFlow* g_dataflow_ptr_; \
extern DLL_API CSendingArray* g_sendingarray_ptr_;\
extern DLL_API CReceivingArray* g_receivingarray_ptr_;\
extern DLL_API CNetworkOperator* g_networkoperator_ptr_; \
extern DLL_API DomainParticipantFactory* g_domainparticipantfactory_ptr_;\
extern DLL_API DomainParticipant* g_domainparticipant_ptr_;\
extern DLL_API CPublisher* g_publisher_ptr_;\
extern DLL_API CSubscriber* g_subscriber_ptr_;

//���̼乲���Port�ŵ�����
extern unsigned short _globalPort;
DLL_API unsigned short GetPortNumber();//ȫ�ַ��� ��ȡ���̼乲��˿ںţ�����˿ں�ÿ������1
DLL_API void SetPortNumber(unsigned short port);//ȫ�ַ��� ���ý�����ʹ�õĶ˿ں�
extern unsigned short PORT_ACCEPT;

#endif