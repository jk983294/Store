/***************************************************
文件名： Configure.h
说明：	一系列参数以及消息类型的定义
作者：   董丹
时间：	2013-08-1
版本：	复赛
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
	/* 消息的发送进程是谁，就填谁的id */
	unsigned src_id;
	/* 消息的接收进程是谁，就填谁的id */
	unsigned dst_id;
	/* 发送“时间请求”消息时填写，
	回复“时间应答”消息时，其值要与请求消息保持一致。 */
	unsigned usr_id;
	/* 消息类型：0, 时间请求；1, 时间答应；2, 心跳请求；3, 心跳应答 */
	unsigned msg_type;
	/* 服务端回复“时间应答”消息时，
	在data中填入当前时间的字符串，形式如“2013-06-20 13:56:28”即可  */
	char data[32];
} t_msg;
const unsigned MSG_TYPE_TIME_REQUST = 0;			//时间请求
const unsigned MSG_TYPE_TIME_REPLY = 1;				//时间答应
const unsigned MSG_TYPE_HEART_REQUST = 2;		//心跳请求
const unsigned MSG_TYPE_HEART_REPLY = 3;			//心跳应答


const string conf_path = "configuration.txt";
const string log_path = "log.txt";
const string ip = "127.0.0.1";
const int CLIENT_SEND_INTERVAL = 5*1000;		//客户端发送时间请求报文间隔
const int HEARTBEAT_SEND_INTERVAL = 0.5*1000;    //每次批量发送心跳的时间间隔

enum ServerCondition{
	GOOD,										//服务器都是好的
	BAD											//4次没有回复心跳，认为服务器不工作，以后选择的时候不会选择它
};

enum LoadBalanceAlogrithm{                  
	DistributedInProportion,					//按比例分发的负载均衡算法
	DistributedInFastest,							//基于最快响应的负载均衡算法
	round_alogrithm								//初赛中的轮转算法
};

enum SessionKeepMethod{
	SrcIdBased,											//基于src_id的会话保持
	UsrIdBased,											//基于usr_id的会话保持
	invalid_session_keep_method               //无效的会话保持功能，不适用会话保持功能
};

//以下为与互斥锁资源相关的工具宏定义
#define MUTEX HANDLE
#define MUTEX_LOCK(mutex_sendto) WaitForSingleObject(mutex_sendto, INFINITE)
#define MUTEX_UNLOCK(mutex_sendto) ReleaseMutex(mutex_sendto)
#define MUTEX_INIT(mutex_sendto) mutex_sendto = CreateMutex(0, FALSE, 0)
#define MUTEX_DESTROY(mutex_sendto) CloseHandle(mutex_sendto)
//线程资源
#define THREADID DWORD
struct CThread{
	THREADID ThreadID;
	HANDLE ThreadHandle;
};
//服务器结构体
typedef struct Peer{
	unsigned id;												//id
	unsigned short port;									//端口号
	ServerCondition condition;						//服务器状态
	unsigned load_weight;								//服务器负载权重
	short send_heartbeat_no_accept_times;    //服务器未响应心跳请求报文的次数
	long reply_time;										//服务器响应时间
}Peer;

typedef struct session_keep_in_server{
	long time;
	unsigned server_id;
}session_keep_in_server;

#endif