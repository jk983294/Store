/***************************************************
文件名： Configure.h
说明：	一系列参数以及消息类型的定义
作者：   董丹
时间：	2013-07-16
版本：	初赛
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
const int HEARTBEAT_TIMEOUT = 10*1000;			//心跳超时时间
const int HEARTBEAT_SEND_INTERVAL = 6*1000;    //每次批量发送心跳的时间间隔

enum ServerCondition{
	GOOD,										//初始认为服务器都是好的
	GOOD_HEARTBEAT_NO_REPLY_YET,               //在心跳发出之前状态是GOOD，发送心跳后没有回复
	BAD_HEARTBEAT_NO_REPLY_YET,				   //在心跳发出之前状态是BAD，该次发送心跳后没有回复
	BAD											//没有回复心跳，改为此状态，以后轮转选择的时候不会选择它
};

//以下为各种和操作系统资源相关的工具宏定义
//互斥锁资源
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
struct Peer{
	unsigned id;
	unsigned short port;
	ServerCondition condition;
};

#endif