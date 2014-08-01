/***************************************************
文件名： server.h
说明：	定义服务端类的头文件
作者：   董丹
时间：	2013-08-1
版本：	复赛
***************************************************/

#ifndef SERVER_H
#define SERVER_H

#include "SocketUtil.h"
#include "Configure.h"

class Server
{
public:
	SOCKET mysocket;								//监听套接字
	unsigned short myport;							//监听端口号
	unsigned id;											//唯一id不能相同
	unsigned int RecvCorrectCount,RecvIncorrectCount,SendCount;	//收到正确报文数，收到错误报文数，发送报文数
	CThread server_recv_thread;					//接收和发送信息的线程
	bool isdebug;										//调试开关
	MUTEX mutex_print;								//向屏幕打印信息的互斥锁
public:
	Server(unsigned myid,	unsigned short port);
	~Server();
	void run();																	//启动主进程，用于接受人工命令
	void run_server_recv_thread();									//接收和发送信息的线程运行程序
private:
	void print_debugInformation(string &s);					//打印debug信息
	void print_debugInformation(char * info);					//打印debug信息，重载
	void print_debugInformation_msg(string &s,t_msg &msg);						//打印debug信息和msg消息
	void print_debugInformation_msgNodate(char * content,t_msg &msg);	//打印debug信息和不带时间字段的msg消息
	void print_statistics();												//打印统计信息
};

#endif