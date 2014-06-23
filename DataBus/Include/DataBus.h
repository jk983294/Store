/**********************************************
该文件为包含了整个系统中的所有相关数据结构（预）
定义，常量定义、宏以及引用文件，由此确定了整个项
目的代码结构。

1、该文件仅针对windowsxp系统。
2、系统中各个类的实现文件均引用该文件。
3、应用组件引用该项目唯一的头文件就是该文件。

创建：2011-12-10
**********************************************/
#ifndef DATABUS_H
#define DATABUS_H

//以下为引用的操作系统头文件
#include <direct.h>
#include <errno.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <winsock.h>

//以下为项目使用的各种常量定义
const int DEFAULT_PORT = 6666; //默认起始分配端口号
const int TOPIC_MAXNUM = 15; //每个组件最大的主题数量
const int PEER_MAXNUM_FOR_EACH_TOPIC = 15; //每个主题最大的订阅数量
const int MAXPEERNUM = 15; //节点最多允许连接的CPeer的数量
const int MULTICAST_PORT = 7905; //发布信息组播端口号    
#define MULTICAST_GROUP "228.4.5.6" //发布信息组播地址
enum protocolType
{
	TCP,
	UDP,
	RAPIDIO
}; //网络封装报文类型
const int TCP_HEADER_LEN = 8; //TCP报文头长度
const int UDP_HEADER_LEN = 16; //组播地址上发布报文的长度
const int HEADER_LEN = 6; //报文头的长度
const int DEFAULT_BUF_SIZE = 1024*8; //原始数据的最大长度

//以下为报文标识
const short MSG_PUBLISH = 196; //主题发布报文
const short MSG_COMPSTATE = 197;//组件工作状态报文
const short MSG_ESTABLISH = 64; //TCP连接握手报文　
const short MSG_SUBSCRIBE = 65; //主题订阅报文　
const short MSG_UNSUBSCRIBE = 66; //取消主题订阅报文
const short MSG_DATA_TCP = 67; //数据传输报文头
const short MSG_UNPUBLISH = 68; //取消主题发布报文 
const int  PACKET_INTERRUPT_CONNECT = 66; //中断select的连接

const int PEERDEADTHRESHOLD = 60; //验证发布信息失效的等待轮数
const int MAX_TOPIC_SIZE=20; //最大主题数目
const int MULTICAST_PERIOD=1; //组播周期 单位为秒

//以下为数据逻辑层的各种参数
const int MAX_ARRAY_LENGTH = 1000; //队列中最大缓存的数据数量

//以下为应用封装层的各种参数
const int MAXOPERATIONUM = 8; //每个数据响应类最多定义的数据响应函数
const int MAXINFOHANDLERNUM = 8; //每个组件最多定义的数据响营类
const int DOMAIN_INTERVAL = 100000; //每个域的topicid间隔
const int MAX_TOPIC_SEGMENTS = 4; //每个主题的最多字段数
const int MAX_TOPIC_SEGLENG = 128; //每个主题字段的长度
const int MAX_CDATA_LENGTH = 256; //每个数据的字段数
const int SUBSCRIBE_MONITOR_INFO=1; //监控订阅主题

#define TopicID int
#define domain_id int

//以下为各种和操作系统资源相关的工具宏定义
//互斥锁资源
#define MUTEX HANDLE
#define SEMA HANDLE
#define MUTEX_LOCK(mutex) WaitForSingleObject(mutex, INFINITE)
#define MUTEX_UNLOCK(mutex) ReleaseMutex(mutex)
#define MUTEX_INIT(mutex) mutex = CreateMutex(0, FALSE, 0)
#define MUTEX_DESTROY(mutex) CloseHandle(mutex)
//信号量资源
#define SEMA_WAIT_TIME(sema,delay) WaitForSingleObject(sema, delay)
#define SEMA_WAIT(sema) WaitForSingleObject(sema, INFINITE)
#define SEMA_POST(sema) ReleaseSemaphore(sema, 1, NULL)
#define SEMA_TRYWAIT(sema) WaitForSingleObject(sema, 0)
#define SEMA_DESTROY(sema) CloseHandle(sema)
#define SEMA_INIT(sema, initCount, maxCount) sema = CreateSemaphore(NULL, initCount, maxCount, NULL) 
//线程资源
#define THREADID DWORD

//以下为DLL调用方式定义以及DLL共享资源
#ifdef DATABUS_EXPORTS   
#define DLL_API __declspec(dllexport) 
#else   
#define DLL_API __declspec(dllimport)   
#endif  

//以下为项目中所有的类预定义
class CThread;
//以下为应用封装层的所有类预定义
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
//以下为逻辑数据层的所有类预定义
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
//以下为网络封装层的所有类预定义
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

//以下为项目中涉及到的所有类定义文件
#include "Thread.h"
#include "CAcceptor.h"
#include "CConnector.h"
#include "CReceiver.h"
#include "CSender.h"
//以下为应用封装层的所有类定义文件
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
//以下为逻辑数据层的所有类定义文件
#include "CDataArray.h"
#include "CDataCache.h"
#include "CDataFlow.h"
#include "CFlowIn.h"
#include "CFlowOut.h"
#include "CReceivingArray.h"
#include "CSendingArray.h"
#include "DataLogics.h"
//以下为网络封装层的所有类定义文件
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

//以下为提供给组件使用的工具函数
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

//进程间共享的Port号的声明
extern unsigned short _globalPort;
DLL_API unsigned short GetPortNumber();//全局方法 获取进程间共享端口号，共享端口号每次增加1
DLL_API void SetPortNumber(unsigned short port);//全局方法 设置进程内使用的端口号
extern unsigned short PORT_ACCEPT;

#endif