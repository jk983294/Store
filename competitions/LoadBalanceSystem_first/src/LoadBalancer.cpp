
/***************************************************
文件名： LoadBalancer.cpp
说明：	负载均衡器类的源文件，定义该类的函数
作者：   董丹
时间：	2013-07-16
版本：	初赛
***************************************************/


#include "LoadBalancer.h"

DWORD WINAPI StartPktThreadAddress(LPVOID param){
	LoadBalancer * thread=(LoadBalancer *) param;
	thread->run_pkt_thread();
	return 0;
}

DWORD WINAPI StartSendHeartbeatThreadAddress(LPVOID param){
	LoadBalancer * thread=(LoadBalancer *) param;
	thread->run_send_heartbeat_thread();
	return 0;
}

LoadBalancer::LoadBalancer(){
	logger = new Logger(log_path);   //日志类初始化
	ClientRecvCorrectCount=ClientRecvIncorrectCount=ClientSendCount = 0;
	ServerRecvCorrectCount=ServerRecvIncorrectCount=ServerSendCount = 0;
	initilize(conf_path);
	initilize_socket();
	MUTEX_INIT(mutex_sendto);
	MUTEX_INIT(mutex_print);
	
}
LoadBalancer::~LoadBalancer(){
	MUTEX_DESTROY(mutex_sendto);
	MUTEX_DESTROY(mutex_print);
	TerminateThread(pkt_thread.ThreadHandle, 0);
	TerminateThread(send_heartbeat_thread.ThreadHandle, 0);
	closesocket(client_socket);
	closesocket(server_socket);
}
void LoadBalancer::initilize_socket(){
	//初始化socket
	if (! SocketUtil::create_udp_socket(client_socket))
		logger->log_error_time_ln("创建client socket错误");
	if(! SocketUtil::udp_bind(client_socket,client_udp_port))
		logger->log_error_time_ln("client socket绑定错误");
	if (! SocketUtil::create_udp_socket(server_socket))
		logger->log_error_time_ln("创建server socket错误");
	if(! SocketUtil::udp_bind(server_socket,server_udp_port))
		logger->log_error_time_ln("server socket绑定错误");
	logger->log_time_ln("初始化client_socket、server_socket完成");
}
void LoadBalancer::initilize(string conf_file){
	ifstream ifs(conf_file.c_str());
	string filepath = "../"+conf_file;
	if (! ifs.good())	ifs.open(filepath.c_str());
	if(ifs.good()){
		string key,value;
		stringstream ss;  
		while (ifs>>key>>value){
			if (key == "client_udp_port"){
				ss.clear();	ss<<value;	ss>>client_udp_port;
			}else if (key == "server_udp_port"){
				ss.clear();	ss<<value;	ss>>server_udp_port;
			}else if (key == "LB_id"){
				ss.clear();	ss<<value;	ss>>id;
			}else if (key == "server_support_count")
			{
				ss.clear();	ss<<value;	ss>>server_support_count;
				choosen_server = server_support_count-1;
			}else if (key == "server_id"){
				Peer newpeer;	newpeer.condition = GOOD;
				ss.clear();	ss<<value;	ss>>newpeer.id;
				ifs>>key>>value;
				ss.clear();	ss<<value;	ss>>newpeer.port;
				servers.push_back(newpeer);
			}
		}
		logger->log_time_ln("读取配置文件成功");
	}else{
		printDebugInformation("读取配置文件失败");
		logger->log_error_time_ln("读取配置文件失败");
	}
}
void LoadBalancer::run(){
	pkt_thread.ThreadHandle = CreateThread(NULL, 0, StartPktThreadAddress, this, 0, &pkt_thread.ThreadID);
	send_heartbeat_thread.ThreadHandle = CreateThread(NULL, 0, StartSendHeartbeatThreadAddress, this, 0, &send_heartbeat_thread.ThreadID);
	int choice;
	bool isEnd = false;
	cout<<"负载均衡端 ( id = "<<id<<" ) 启动..."<<endl
		<<"按1 打开debug开关"<<endl
		<<"按2 关闭debug开关"<<endl
		<<"按3 显示统计数据"<<endl
		<<"按4 关闭负载均衡程序"<<endl;
	while (cin>>choice){		
		switch(choice){
		case 1:	isdebug = true;	 logger->log_time_ln("打开debug开关");	break;
		case 2: isdebug = false; logger->log_time_ln("关闭debug开关");	break;
		case 3: print_statistics(); break;
		case 4: printDebugInformation("关闭负载均衡程序...");	logger->log_time_ln("关闭负载均衡程序...");	isEnd = true;	break;
		default: printDebugInformation("没有该指令");	break;
		}
		if (isEnd)	break;
	}
}
void LoadBalancer::run_pkt_thread(){
	int ret;
	t_msg msg;	
	string content;
	unsigned srcid;
	stringstream ss;
	struct sockaddr_in from_,server_addr;			//对方信息	
	server_addr.sin_family=AF_INET;    
	server_addr.sin_addr.s_addr=inet_addr(ip.c_str());
	int BUFFER_LEN = sizeof(t_msg),	ADDR_LEN = sizeof(from_);
	fd_set fdread; //维持一个SOCKET数组
	while (true)
	{
		FD_ZERO(&fdread);
		FD_SET(client_socket,&fdread);		//监听client_socket
		FD_SET(server_socket,&fdread);		//监听server_socket
		ret = select(0, &fdread, NULL, NULL, NULL);
		//处理client发来的请求
		if (client_socket 	&&	(FD_ISSET(client_socket, &fdread))){
			if(recvfrom(client_socket, (char*)&msg, BUFFER_LEN, 0, (struct sockaddr *) &from_, (int *)&ADDR_LEN) < 0){
				logger->log_error_time_ln("从client处接收错误");
			}else{
				if (msg.dst_id == id){							
					if (msg.msg_type == MSG_TYPE_TIME_REQUST){	//时间请求报文	
						++ClientRecvCorrectCount;
						if(isdebug)
						{
							ss<<"正确接收从Client端发来的时间请求报文：";
							content = ss.str();
							ss.str("");
							printDebugInformation_NoDateMsg(content,msg);
						}
						clients_usrid_addr[msg.usr_id] = from_;					//用户可能在别的socket上登陆，直接覆盖原来的记录
						clients_usrid_dstid[msg.usr_id]	= msg.src_id;
						pick_server();						//轮转法选择一个服务器
						if (choosen_server	<	0){
							ss<<"找不到服务器处理该请求";	content = ss.str();	ss.str(""); 
							logger->log_time_ln(content);
							if(isdebug)	
								printDebugInformation(content);
						}else{
							server_addr.sin_port = htons(servers[choosen_server].port);  
							msg.dst_id = servers[choosen_server].id;	msg.src_id = id;
							if(send_data_thread_safe(msg,BUFFER_LEN,server_addr,ADDR_LEN) < 0){
								ss<<"发送时间请求报文给服务器  "<<msg.dst_id<<" 错误  ";	content = ss.str();	ss.str(""); 
								if(isdebug) perror(content.c_str());
								logger->log_error_time_ln(content);
							}
							else{	//正确发送时间请求报文
								++ServerSendCount;
								ss<<"发送客户( usr_id = "<<msg.usr_id<<" ) 的时间请求报文给服务器: "<<servers[choosen_server].id<<" 成功";
								content = ss.str();	ss.str(""); 
								logger->log_time_ln(content);
								if(isdebug){
									printDebugInformation_NoDateMsg(content,msg);
								}
							}
						}
						
					}
					else if (msg.msg_type == MSG_TYPE_HEART_REQUST){	//心跳请求
						++ClientRecvCorrectCount;	
						msg.dst_id = msg.src_id; msg.src_id = id; msg.msg_type = MSG_TYPE_HEART_REPLY;
						if(sendto(server_socket, (char*)&msg, BUFFER_LEN, 0, (struct sockaddr *) &from_, ADDR_LEN) < 0){
							ss<<"发送心跳回复报文报文给客户 (usr_id =  "<<msg.usr_id<<") 错误  ";	content = ss.str();	ss.str(""); 
							if(isdebug) perror(content.c_str());
							logger->log_error_time_ln(content);
						}
						else{	//正确发送心跳报文
							++ServerSendCount;		
							ss<<"发送心跳回复报文报文给客户 (usr_id =  "<<msg.usr_id<<") 成功  ";	content = ss.str();	ss.str(""); 
							logger->log_time_ln(content);
							if(isdebug)	printDebugInformation(content);	
						}
					}
					else{//未定义消息类型报文处理
						++ClientRecvIncorrectCount;
						ss<<"从客户端收到未定义消息类型的报文";	content = ss.str();	ss.str(""); 
						if(isdebug) printDebugInformation(content);
						logger->log_time_ln(content);
					}
				}else{	//dst_id != id
					++ClientRecvIncorrectCount;
					ss<<"从客户端收到目标地址不是我的报文";	content = ss.str();	ss.str(""); 
					if(isdebug) printDebugInformation(content);
					logger->log_time_ln(content);
				}
			}
		}
		if (server_socket 	&&	(FD_ISSET(server_socket, &fdread))){
			if(recvfrom(server_socket, (char*)&msg, BUFFER_LEN, 0, (struct sockaddr *) &from_, (int *)&ADDR_LEN) < 0){
				logger->log_error_time_ln("从服务器处接收错误");
			}else{
				if (msg.dst_id == id){						
					if (msg.msg_type == MSG_TYPE_TIME_REPLY){	//时间请求回复报文	
						++ServerRecvCorrectCount;	
						if(isdebug)
						{

							ss<<"正确接收从服务端发来的时间答应报文：";
							content = ss.str(); ss.str("");
							printDebugInformation_msg(content,msg);
						}
						srcid = msg.src_id;
						msg.dst_id = clients_usrid_dstid[msg.usr_id];		msg.src_id = id;
						if(sendto(client_socket, (char*)&msg, BUFFER_LEN, 0, (struct sockaddr *) &clients_usrid_addr[msg.usr_id], ADDR_LEN) < 0){
							ss<<"发送服务器 "<<srcid<<" 的时间回复报文给客户端 ( usr_id = "<<msg.usr_id<<" ) 错误 ";	content = ss.str();	ss.str(""); 
							if(isdebug) perror(content.c_str());
							logger->log_error_time_ln(content);
						}
						else{	//正确发送时间回复报文
							++ClientSendCount;
							ss<<"发送服务器 "<<srcid<<" 的时间回复报文给客户端 ( usr_id = "<<msg.usr_id<<" ) 成功: ";	content = ss.str();	ss.str(""); 
							logger->log_time_ln(content);
							if(isdebug)	{
								printDebugInformation_msg(content,msg);
							}
						}
					}
					else if (msg.msg_type == MSG_TYPE_HEART_REPLY){	//心跳应答报文
						++ServerRecvCorrectCount;	
						if(isdebug)
						{
							ss<<"正确接收从服务端 ( id = "<<msg.src_id<<" )发来的心跳答应报文：";
							content = ss.str(); ss.str("");
							printDebugInformation_HeartBeatMsg(content,msg);
							
						}
						for(int i = 0; i < server_support_count; i++)
						{
							if(servers[i].id == msg.src_id)
							{
								servers[i].condition = GOOD; //修改服务器的状态
								break;
							}
						}
					}
					else{	//未定义消息类型报文处理
						++ServerRecvIncorrectCount;
						ss<<"从服务器端收到未定义消息类型的报文";	content = ss.str();	ss.str(""); 
						if(isdebug) printDebugInformation(content);
						logger->log_time_ln(content);
					}
				}else{	//dst_id != id
					++ServerRecvIncorrectCount;
					ss<<"从服务器端收到目标地址不是我的报文";	content = ss.str();	ss.str(""); 
					if(isdebug) printDebugInformation(content);
					logger->log_time_ln(content);
				}
			}
		}
	}
}
void LoadBalancer::run_send_heartbeat_thread(){
	Sleep(2*1000);
	t_msg msg;
	string content;
	stringstream ss;
	struct sockaddr_in server_addr;			//对方信息	
	server_addr.sin_family=AF_INET;    
	server_addr.sin_addr.s_addr=inet_addr(ip.c_str());
	int BUFFER_LEN = sizeof(t_msg),	ADDR_LEN = sizeof(server_addr);
	while(true)
	{
		for(int i = 0; i < server_support_count; i++)
		{
			server_addr.sin_port = htons(servers[i].port);
			msg.dst_id = servers[i].id;
			msg.src_id = id;
			msg.msg_type = MSG_TYPE_HEART_REQUST;
			if(send_data_thread_safe(msg,BUFFER_LEN,server_addr,ADDR_LEN) < 0){
				 ss<<"发送心跳请求报文给服务器  "<<msg.dst_id<<" 错误  ";	content = ss.str();	ss.str(""); 
				if(isdebug) perror(content.c_str());
				logger->log_error_time_ln(content);
				servers[i].condition = BAD;
			}
			else{	//正确发送心跳请求报文
				if(servers[i].condition == BAD)
				    servers[i].condition = BAD_HEARTBEAT_NO_REPLY_YET;
				else if(servers[i].condition == GOOD)
					servers[i].condition = GOOD_HEARTBEAT_NO_REPLY_YET;
				++ServerSendCount;
				ss<<"发送心跳请求报文给服务器 "<<servers[i].id<<" 成功: ";
				content = ss.str();	ss.str(""); 
				logger->log_time_ln(content);
				if(isdebug){
					printDebugInformation_HeartBeatMsg(content,msg);
				}
			}

		}
		Sleep(HEARTBEAT_TIMEOUT);
		for(int j = 0;j < server_support_count;j++)
		{
			
			if(servers[j].condition != GOOD)
			{
				servers[j].condition = BAD;
				if(isdebug)
				{
					ss<<servers[j].id<<"号服务器失效！";
				    content = ss.str();	ss.str(""); 
					printDebugInformation(content);
				}
				    
			}
			
		}
		Sleep(HEARTBEAT_SEND_INTERVAL);
		
	}

}
int LoadBalancer::send_data_thread_safe(t_msg &msg,int &sendlen,struct sockaddr_in &targetaddr,int &addrlen){
	int send_success_length=0;
	MUTEX_LOCK(mutex_sendto);
	send_success_length =sendto(server_socket, (char*)&msg, sendlen, 0, (struct sockaddr *) &targetaddr, addrlen);
	MUTEX_UNLOCK(mutex_sendto);
	return send_success_length;	
}
void LoadBalancer::pick_server(){
	
	if(server_support_count <= 0){	choosen_server = -1;	return;}
	int last;
	if(choosen_server == -1)
		last = server_support_count-1;
	else
		last = choosen_server;
	do 
	{
		++choosen_server;
		choosen_server %= server_support_count;
		if(servers[choosen_server].condition == GOOD || servers[choosen_server].condition == GOOD_HEARTBEAT_NO_REPLY_YET)	return;
	} while (choosen_server != last);
	
	choosen_server = -1;		//没有可用的服务器
	
}

void LoadBalancer::printDebugInformation_msg(string &s,t_msg &msg)
{
	MUTEX_LOCK(mutex_print);
	cout<<s<<endl;
	cout<<"src_id: "<<msg.src_id<<" usr_id: "<<msg.usr_id<<" dst_id: "<<msg.dst_id
		<<" msg_type: "<<msg.msg_type<<" data: "<<msg.data<<endl<<endl;
	MUTEX_UNLOCK(mutex_print);

}

void LoadBalancer::printDebugInformation_HeartBeatMsg(string &s,t_msg &msg)
{
	MUTEX_LOCK(mutex_print);
	cout<<s;
	cout<<"src_id: "<<msg.src_id<<" dst_id: "<<msg.dst_id
		<<" msg_type: "<<msg.msg_type<<endl<<endl;
	MUTEX_UNLOCK(mutex_print);
}
void LoadBalancer::printDebugInformation_NoDateMsg(string &s,t_msg &msg)
{
	MUTEX_LOCK(mutex_print);
	cout<<s<<endl;
	cout<<"src_id: "<<msg.src_id<<" usr_id: "<<msg.usr_id<<" dst_id: "<<msg.dst_id
		<<" msg_type: "<<msg.msg_type<<endl<<endl<<endl;
	MUTEX_UNLOCK(mutex_print);

}
void LoadBalancer::printDebugInformation(string &s)
{
	MUTEX_LOCK(mutex_print);
	cout<<s<<endl<<endl;
	MUTEX_UNLOCK(mutex_print);
}
void LoadBalancer::printDebugInformation(char * info)
{
	MUTEX_LOCK(mutex_print);
	cout<<info<<endl<<endl;
	MUTEX_UNLOCK(mutex_print);
}


void LoadBalancer::print_statistics(){
	MUTEX_LOCK(mutex_print);
	stringstream ss;
	ss<<"收到客户端报文数: "<<ClientRecvCorrectCount+ClientRecvIncorrectCount<<" (收到客户端正确报文数: "<<ClientRecvCorrectCount<<" 收到客户端错误报文数: "<<ClientRecvIncorrectCount
		<<") 发送客户端报文数: "<<ClientSendCount<<"收到服务端报文数: "<<ServerRecvCorrectCount+ServerRecvIncorrectCount<<" (收到服务器端正确报文数: "<<ServerRecvCorrectCount
		<<" 收到服务器端错误报文数: "<<ServerRecvIncorrectCount<<") 发送服务器端报文数: "<<ServerSendCount;
	string statistics = ss.str();
	logger->log_time_ln(statistics);	
	cout<<statistics<<endl;
	MUTEX_UNLOCK(mutex_print);
}