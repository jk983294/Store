/***************************************************
文件名： LoadBalancer.h
说明：	负载均衡器类的头文件
作者：   董丹
时间：	2013-08-1
版本：	复赛
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
	SOCKET client_socket,	server_socket;									//收发客户端、服务器端的套接字
	unsigned short client_udp_port,	server_udp_port;				//监听客户端、服务器端的端口
	unsigned id;																			//唯一id
	int server_support_count;														//支持服务器数
	LoadBalanceAlogrithm load_balance_algorithm;                   //负载均衡算法
	SessionKeepMethod session_keep_method;                         //会话保持功能
	int choosen_server;																//轮转法选择的服务器下标
	vector<Peer> servers;															//服务器信息
	map<unsigned, struct session_keep_in_server> usrid_serverid;				//保存客户端的usrid和分配服务器id和会话保持时间的映射
	map<unsigned, struct session_keep_in_server> srcid_serverid;				//保存客户端的src_id和分配服务器id和会话保持时间的映射
	map<unsigned, struct sockaddr_in> clients_usrid_addr;						//保存客户端的usrid和地址的映射
	map<unsigned, unsigned>	clients_usrid_dstid;									//保存客户端的usrid和id的映射
	unsigned int ClientRecvCorrectCount,ClientRecvIncorrectCount,ClientSendCount;	//客户端，收到正确报文数，收到错误报文数，发送报文数
	unsigned int ServerRecvCorrectCount,ServerRecvIncorrectCount,ServerSendCount;	//服务器端，收到正确报文数，收到错误报文数，发送报文数
	CThread pkt_thread;										//收发报文线程
	CThread send_heartbeat_thread;					//定时发送心跳报文线程
	CThread detect_thread;									//检测服务器是否出现故障以及会话是否超时
	Logger *logger;												//日志
	bool isdebug;												//调试开关
	MUTEX mutex_sendto;									//发送心跳请求和时间请求两线程的互斥锁
	MUTEX mutex_print;										//向屏幕打印消息的互斥锁
	MUTEX mutex_session_modify;						//修改会话条目的锁
public: 
	LoadBalancer();
	~LoadBalancer();
	void initilize_socket();															//初始化client_socket和server_socket		
	void initilize(string conf_file);													//读取配置文件
	void run();																				//主进程，主要用来接收人工指令
	void run_pkt_thread();															//收发报文线程主体
	void run_send_heartbeat_thread();										//发送心跳报文的线程
	void run_detect_thread();														//检测线程的主体
	//保证回复心跳和发送时间请求两个线程的调用线程安全
	int send_data_thread_safe(t_msg &msg,int &sendlen,struct sockaddr_in &targetaddr,int &addrlen);	
	void pick_server(LoadBalanceAlogrithm load_balance_algorithm);		//根据某一负载均衡算法选取服务器	
	void pick_server_session(t_msg &msg);												//支持会话保持功能时计算选取的服务器
private: 
	void pick_server_round();							//轮转算法
	void pick_server_proportion();					//按比例分发负载均衡算法
	void pick_server_fastest();							//基于最快相应的负载均衡算法
	void print_statistics();								//打印统计信息														                                                 
	void printDebugInformation(string &s);    //打印debug信息
	void printDebugInformation(char * info);  //打印debug信息    
	void printDebugInformation_msg(string &s,t_msg &msg);					//打印debug信息和msg消息
	void printDebugInformation_NoDateMsg(string &s,t_msg &msg);		//打印debug信息和不带时间字段的msg消息
	void printDebugInformation_HeartBeatMsg(string &s,t_msg &msg);		//打印debug信息和心跳报文消息
};


#endif