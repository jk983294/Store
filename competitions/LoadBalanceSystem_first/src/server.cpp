/***************************************************
文件名： server.cpp
说明：	定义服务端类的源文件
作者：   董丹
时间：	2013-07-16
版本：	初赛
***************************************************/
#include "server.h"


DWORD WINAPI StartServerRecvThreadAddress(LPVOID param){
	Server * thread=(Server *) param;
	thread->run_server_recv_thread();
	return 0;
}
Server::Server(unsigned myid,	unsigned short port){
	id = myid;		myport = port;
	RecvCorrectCount=RecvIncorrectCount=SendCount = 0;
	SocketUtil::create_udp_socket(mysocket);
	if(! SocketUtil::udp_bind(mysocket,myport))
		print_debugInformation("server socket绑定错误");
	isdebug = true;
	MUTEX_INIT(mutex_print);
}
Server::~Server()
{
	TerminateThread(server_recv_thread.ThreadHandle, 0);
	closesocket(mysocket);
	MUTEX_DESTROY(mutex_print);
}
void Server::run_server_recv_thread(){
	Sleep(10);
	unsigned tmpid,srcid;
	struct sockaddr_in from_;
	t_msg msg;
	int BUFFER_LEN = sizeof(t_msg),	ADDR_LEN = sizeof(from_);
	stringstream ss;	string content;
	while (true)
	{
		if(recvfrom(mysocket, (char*)&msg, BUFFER_LEN, 0, (struct sockaddr *) &from_, (int *)&ADDR_LEN) < 0){
			if(isdebug) perror("从负载均衡器处接收错误: ");
		}
		else{
			if (msg.dst_id == id)
			{
				if (msg.msg_type == MSG_TYPE_TIME_REQUST){	//时间请求报文
					if(isdebug){	
		
						print_debugInformation_msgNodate("成功接收到时间请求报文: ",msg);
					}

					++RecvCorrectCount;			
					strcpy((char *)(msg.data),timeUtil::currentTime().c_str());

					tmpid = msg.dst_id;		msg.dst_id = msg.src_id;	msg.src_id = tmpid; msg.msg_type = MSG_TYPE_TIME_REPLY;
					if(sendto(mysocket, (char*)&msg, BUFFER_LEN, 0, (struct sockaddr *) &from_, ADDR_LEN) < 0){
						ss<<"发送时间回复报文给客户端 ( usr_id = "<<msg.usr_id<<" ) 错误 ";	content = ss.str();	ss.str(""); 
						if(isdebug) perror(content.c_str());
					}
					else{
						++SendCount;
						if(isdebug){ 
							
							ss<<"回复客户端 ( usr_id = "<<msg.usr_id<<" ) 的应答时间请求报文成功: ";
							content = ss.str();
							ss.str("");
							print_debugInformation_msg(content,msg);
						}
						
					}
				}
				else if (msg.msg_type == MSG_TYPE_HEART_REQUST){	//心跳请求
					if(isdebug){	
						
						print_debugInformation("成功接收到心跳请求报文!");
						//printNoData(msg);
					}
					++RecvCorrectCount;	
					srcid = msg.src_id; msg.dst_id = msg.src_id; msg.src_id = id; msg.msg_type = MSG_TYPE_HEART_REPLY;
					if(sendto(mysocket, (char*)&msg, BUFFER_LEN, 0, (struct sockaddr *) &from_, ADDR_LEN) < 0){
						ss<<"发送心跳回复报文报文给负载均衡器 "<<srcid<<" 错误  ";	content = ss.str();	ss.str(""); 
						if(isdebug) perror(content.c_str());
					}
					else{	//正确发送心跳报文
						++SendCount;		
						ss<<"发送心跳回复报文报文给负载均衡器 "<<srcid<<" 成功  ";	content = ss.str();	ss.str(""); 
						if(isdebug)	print_debugInformation(content);	
					}
				}
				else{	//未定义消息类型报文处理
					++RecvIncorrectCount;
					if(isdebug) print_debugInformation("从负载均衡器收到未定义消息类型的报文");
				}
			}else{
				++RecvIncorrectCount;
				if(isdebug) print_debugInformation("从负载均衡器收到目标地址不是我的报文");
			}
		}
	}
}
void Server::run(){
	server_recv_thread.ThreadHandle = CreateThread(NULL, 0, StartServerRecvThreadAddress, this, 0, &server_recv_thread.ThreadID);
	int choice;
	bool isEnd = false;
	cout<<"服务器 (id = "<<id<<" ) 启动..."<<endl
		<<"按1 打开debug开关"<<endl
		<<"按2 关闭debug开关"<<endl
		<<"按3 显示统计数据"<<endl
		<<"按4 关闭服务器程序"<<endl;
	while (cin>>choice)
	{		
		switch(choice){
		case 1:	isdebug = true;		break;
		case 2: isdebug = false;	break;
		case 3: print_statistics();	break;
		case 4: print_debugInformation("关闭服务器程序...");		isEnd = true;	break;
		default: print_debugInformation("没有该指令");	break;
		}
		if (isEnd)	break;
	}
}

void Server::print_statistics(){
	MUTEX_LOCK(mutex_print);
	int totalRecvCount = RecvCorrectCount+RecvIncorrectCount;
	cout<<"共接受报文数："<<totalRecvCount<<" (收到正确报文数: "<<RecvCorrectCount<<" 收到错误报文数: "<<RecvIncorrectCount
		<<")"<<" 应答报文数: "<<SendCount<<endl;
	MUTEX_UNLOCK(mutex_print);
}
void Server::print_debugInformation(string &s)
{
	MUTEX_LOCK(mutex_print);
	cout<<s<<endl;
	MUTEX_UNLOCK(mutex_print);

}
void Server::print_debugInformation(char * info)
{
	MUTEX_LOCK(mutex_print);
	cout<<info<<endl;
	MUTEX_UNLOCK(mutex_print);

}
void Server::print_debugInformation_msg(string &s,t_msg &msg)
{
	MUTEX_LOCK(mutex_print);
	cout<<s<<endl;
	cout<<"src_id: "<<msg.src_id<<" usr_id: "<<msg.usr_id<<" dst_id: "<<msg.dst_id
		<<" msg_type: "<<msg.msg_type<<" data: "<<msg.data<<endl;
	MUTEX_UNLOCK(mutex_print);

}
void Server::print_debugInformation_msgNodate(char * content,t_msg &msg)
{
	MUTEX_LOCK(mutex_print);
	cout<<content<<endl;
	cout<<"src_id: "<<msg.src_id<<" usr_id: "<<msg.usr_id<<" dst_id: "<<msg.dst_id
		<<" msg_type: "<<msg.msg_type<<endl;
	MUTEX_UNLOCK(mutex_print);

}