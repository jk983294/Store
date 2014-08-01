/***************************************************
文件名： client.h
说明：	定义客户端类的头文件
作者：   董丹
时间：	2013-07-16
版本：	初赛
***************************************************/
#ifndef CLIENT_H
#define CLIENT_H

#include "SocketUtil.h"
#include "Configure.h"

class Client
{
public:
	SOCKET mysocket;										//发送监听socket
	unsigned short lb_port;					//负载均衡port 自己监听port号
	unsigned id	,	usr_id	,	dst_id;					//id可以相同，usr_id不能相同	
	unsigned n;													//发送请求报文次数
	unsigned int RecvCorrectCount,RecvIncorrectCount,SendCount;	//收到正确报文数，收到错误报文数，发送报文数
	CThread client_send_thread,	client_recv_thread;	  //发送时间请求报文和接收时间应答报文的线程
	bool isdebug;    //调试开关
	MUTEX mutex_sendto;     //两个线程发送消息的互斥锁
	MUTEX mutex_print;      //向屏幕打印消息的互斥锁
public:
	Client(unsigned id,unsigned usr_id, unsigned n);
	~Client();
	void initilize(string conf_file);
	void run();                     //主进程程序，接收人工命令
	void run_client_send_thread();  //发送时间请求报文的线程主体
	void run_client_recv_thread();  //接收时间应答报文的线程主体
	//保证回复心跳和发送时间请求两个线程的调用线程安全
	int send_data_thread_safe(t_msg &msg,int &sendlen,struct sockaddr_in &targetaddr,int &addrlen);	
private:
	void print_debugInformation(string &s);   //打印debug信息
	void print_debugInformation(char * info); //打印debug信息
	void print_debugInformation_msg(char * content,t_msg &msg);       //打印debug信息和msg消息
	void print_debugInformation_msgNodate(char * content,t_msg &msg); //打印debug信息和不带时间字段的msg消息
	void print_statistics(); //打印统计信息
	
};




#endif