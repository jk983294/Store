
/***************************************************
文件名： LoadBalancer.cpp
说明：	负载均衡器类的源文件，定义该类的函数
作者：   董丹
时间：	2013-08-1
版本：	复赛
***************************************************/


#include "LoadBalancer.h"
#include <stdlib.h>
using namespace std;

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
DWORD WINAPI StartDetectThreadAddress(LPVOID param){
	LoadBalancer * thread=(LoadBalancer *) param;
	thread->run_detect_thread();
	return 0;
}
LoadBalancer::LoadBalancer(){
	srand((unsigned)time(NULL));
	logger = new Logger(log_path);   //日志类初始化
	ClientRecvCorrectCount=ClientRecvIncorrectCount=ClientSendCount = 0;
	ServerRecvCorrectCount=ServerRecvIncorrectCount=ServerSendCount = 0;
	initilize(conf_path);
	initilize_socket();
	MUTEX_INIT(mutex_sendto);
	MUTEX_INIT(mutex_print);
	MUTEX_INIT(mutex_session_modify);
}
LoadBalancer::~LoadBalancer(){
	MUTEX_DESTROY(mutex_sendto);
	MUTEX_DESTROY(mutex_print);
	MUTEX_DESTROY(mutex_session_modify);
	TerminateThread(pkt_thread.ThreadHandle, 0);
	TerminateThread(send_heartbeat_thread.ThreadHandle, 0);
	TerminateThread(detect_thread.ThreadHandle, 0);
	closesocket(client_socket);
	closesocket(server_socket);
	delete logger; logger = NULL;
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
		unsigned short type;
		while (ifs>>key>>value){
			if (key == "client_udp_port"){
				ss.clear();	ss<<value;	ss>>client_udp_port;
			}else if (key == "server_udp_port"){
				ss.clear();	ss<<value;	ss>>server_udp_port;
			}else if (key == "LB_id"){
				ss.clear();	ss<<value;	ss>>id;
			}else if (key == "server_support_count"){
				ss.clear();	ss<<value;	ss>>server_support_count;
				choosen_server = server_support_count-1;
			}else if (key == "server_id"){
				Peer newpeer;	newpeer.condition = GOOD;
				newpeer.send_heartbeat_no_accept_times = 0;
				newpeer.reply_time = 0;
				ss.clear();	ss<<value;	ss>>newpeer.id;
				ifs>>key>>value;
				ss.clear();	ss<<value;	ss>>newpeer.port;
				ifs>>key>>value;
				ss.clear(); ss<<value;  ss>>newpeer.load_weight;
				servers.push_back(newpeer);
			}else if(key == "load_balance_algorithm"){
				ss.clear(); ss<<value; ss>>type;
				if(type == 0){
				    load_balance_algorithm = DistributedInProportion;
					if(isdebug)	printDebugInformation("负载均衡算法为：按比例分发的负载均衡算法！");
					logger->log_time_ln("负载均衡算法为：按比例分发的负载均衡算法！");	
				}else if(type == 1){
					load_balance_algorithm = DistributedInFastest;
					if(isdebug)	printDebugInformation("负载均衡算法为：基于最快响应的负载均衡算法！");
					logger->log_time_ln("负载均衡算法为：基于最快响应的负载均衡算法！");
				}else{
					load_balance_algorithm = round_alogrithm;
					if(isdebug)	printDebugInformation("负载均衡算法为：轮转算法！！");
					logger->log_time_ln("负载均衡算法为：轮转算法！");	
				}
			}else if(key == "session_keep_method"){
				ss.clear(); ss<<value; ss>>type;
				if(type == 0){
				    session_keep_method = SrcIdBased;
					if(isdebug)	printDebugInformation("会话保持：基于src_id的会话保持！");
					logger->log_time_ln("会话保持：基于src_id的会话保持！");
				}else if(type == 1){
					session_keep_method = UsrIdBased;
					if(isdebug)	printDebugInformation("会话保持：基于usr_id的会话保持！");
					logger->log_time_ln("会话保持：基于usr_id的会话保持！");
				}else{
					session_keep_method = invalid_session_keep_method;
					if(isdebug)	printDebugInformation("不使用任何会话保持机制！");
					logger->log_time_ln("不使用任何会话保持机制！");
				}
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
	detect_thread.ThreadHandle = CreateThread(NULL, 0, StartDetectThreadAddress, this, 0, &detect_thread.ThreadID);
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
						bool invaid_alogrithm;
						if(isdebug){
							ss<<"正确接收从Client端发来的时间请求报文：";
							content = ss.str();		ss.str("");
							printDebugInformation_NoDateMsg(content,msg);
						}
						clients_usrid_addr[msg.usr_id] = from_;					//用户可能在别的socket上登陆，直接覆盖原来的记录
						clients_usrid_dstid[msg.usr_id]	= msg.src_id;				
						pick_server_session(msg);                      //选择服务器：考虑会话保持功能
						if (choosen_server	<	0){
							ss<<"找不到服务器处理该请求";	content = ss.str();	ss.str(""); 
							logger->log_time_ln(content);
							if(isdebug)	printDebugInformation(content);
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
								if(isdebug)	printDebugInformation_NoDateMsg(content,msg);
							}
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
						if(isdebug){
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
						}else{	//正确发送时间回复报文
							++ClientSendCount;
							ss<<"发送服务器 "<<srcid<<" 的时间回复报文给客户端 ( usr_id = "<<msg.usr_id<<" ) 成功: ";	content = ss.str();	ss.str(""); 
							logger->log_time_ln(content);
							if(isdebug)	printDebugInformation_msg(content,msg);
						}
					}
					else if (msg.msg_type == MSG_TYPE_HEART_REPLY){	//接收心跳应答报文
						++ServerRecvCorrectCount;
						//查看当前时间 计算响应时间
						time_t now = time(NULL);
						time_t send_time;
						char *data = msg.data;
						ss.clear(); ss << data; ss >> send_time; ss.str("");	        
						for(int i = 0; i < server_support_count; i++){
							if(servers[i].id == msg.src_id){							
								servers[i].reply_time = now - send_time;        //更新响应时间
							    servers[i].send_heartbeat_no_accept_times = 0;  //收到心跳之后，把连续收不到的次数置0
								break;
							}	
						}
						if(isdebug){
							ss.clear();	ss<<"正确接收从服务端 ( id = "<<msg.src_id<<" )发来的心跳答应报文：";
							content = ss.str(); ss.str("");
							printDebugInformation_HeartBeatMsg(content,msg);
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
		Sleep(HEARTBEAT_SEND_INTERVAL);
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
				servers[i].send_heartbeat_no_accept_times++;
				++ServerSendCount;
				ss<<"发送心跳请求报文给服务器 "<<servers[i].id<<" 成功: ";
				content = ss.str();	ss.str(""); 
				logger->log_time_ln(content);
				if(isdebug)	printDebugInformation_HeartBeatMsg(content,msg);
			}
		}	
	}
}

void LoadBalancer::run_detect_thread(){
	stringstream ss;
	string content;
	while(true){
		//检测服务器的状态
		for(int i = 0; i < server_support_count; i++){
			//服务器由坏变好的检测
			if(servers[i].condition == BAD && servers[i].send_heartbeat_no_accept_times < 4) {
				servers[i].condition = GOOD;
				ss<<servers[i].id<<"号服务器恢复正常！";
				content = ss.str();	ss.str(""); 
				if(isdebug)	printDebugInformation(content);
				logger->log_time_ln(content);
			}
			//服务器由好变坏的检测
			if(servers[i].condition == GOOD && servers[i].send_heartbeat_no_accept_times > 4){
				servers[i].condition = BAD;
				ss<<servers[i].id<<"号服务器失效！";
				content = ss.str();	ss.str(""); 
				if(isdebug)	printDebugInformation(content);
				logger->log_time_ln(content);
			}
		}
		//检测客户端会话是否超时
		map<unsigned, struct session_keep_in_server>::iterator it; 
		if(session_keep_method == SrcIdBased){
			MUTEX_LOCK(mutex_session_modify);
			for(it = srcid_serverid.begin(); it != srcid_serverid.end(); ){
				if(timeUtil::getSecondlong()-(it->second).time >= 30){   //删除会话条目
					ss<<"src_id为 "<<it->first<<" 的会话条目被删除！";
			    	content = ss.str();	ss.str("");
					if(isdebug)	printDebugInformation(content);
					logger->log_time_ln(content);
					srcid_serverid.erase(it++);
				}
				else	++it;
			}
			MUTEX_UNLOCK(mutex_session_modify);
		}else if(session_keep_method == UsrIdBased){
			MUTEX_LOCK(mutex_session_modify);
			for(it = usrid_serverid.begin(); it != usrid_serverid.end(); ){
				if(timeUtil::getSecondlong()-(it->second).time >= 30) {  //删除会话条目	
					ss<<"usr_id为 "<<it->first<<" 的会话条目被删除！";
			    	content = ss.str();	ss.str(""); 
					if (isdebug)	printDebugInformation(content);
					logger->log_time_ln(content);
					usrid_serverid.erase(it++);
				}
				else	++it;
			}
			MUTEX_UNLOCK(mutex_session_modify);
		}
	}	
}
int LoadBalancer::send_data_thread_safe(t_msg &msg,int &sendlen,struct sockaddr_in &targetaddr,int &addrlen){
	int send_success_length=0;
	MUTEX_LOCK(mutex_sendto);
	if(msg.msg_type == MSG_TYPE_HEART_REQUST){
		strcpy((char *)(msg.data),timeUtil::getSecondstring().c_str());
		send_success_length =sendto(server_socket, (char*)&msg, sendlen, 0, (struct sockaddr *) &targetaddr, addrlen);
	}
	else
		send_success_length =sendto(server_socket, (char*)&msg, sendlen, 0, (struct sockaddr *) &targetaddr, addrlen);
	MUTEX_UNLOCK(mutex_sendto);
	return send_success_length;	
}


void LoadBalancer::pick_server_session(t_msg &msg){
	MUTEX_LOCK(mutex_session_modify);
	if(session_keep_method == SrcIdBased){
		//已经存在该会话条目
		if(srcid_serverid.count(msg.src_id) == 1 && servers[srcid_serverid[msg.src_id].server_id].condition == GOOD)    //若已存在该会话且服务器是好的
		{
			choosen_server = srcid_serverid[msg.src_id].server_id;
			//更新最新活跃时间
			srcid_serverid[msg.src_id].time = timeUtil::getSecondlong();
		}else{
			pick_server(load_balance_algorithm);	//根据选定的负载均衡算法选择一个服务器
			if (choosen_server	>=	0){
				//添加会话记录,更新会话保持的时间
				srcid_serverid[msg.src_id].server_id = choosen_server;
				srcid_serverid[msg.src_id].time = timeUtil::getSecondlong();
			}
		}
	}else if(session_keep_method == UsrIdBased){
		//已经存在该会话条目
		if(usrid_serverid.count(msg.usr_id) == 1 && servers[usrid_serverid[msg.usr_id].server_id].condition == GOOD)    //若已存在该会话且服务器是好的
		{
			choosen_server = usrid_serverid[msg.usr_id].server_id;
			//更新最新活跃时间
			usrid_serverid[msg.usr_id].time = timeUtil::getSecondlong();
		}else{
			pick_server(load_balance_algorithm);	//根据选定的负载均衡算法选择一个服务器
			if (choosen_server	>=	0){
				//添加会话记录,更新会话保持的时间
				usrid_serverid[msg.usr_id].server_id = choosen_server;
				usrid_serverid[msg.usr_id].time = timeUtil::getSecondlong();
			}
		}
	}else	pick_server(load_balance_algorithm);	   //根据选定的负载均衡算法选择一个服务器
	MUTEX_UNLOCK(mutex_session_modify);
} 


void LoadBalancer::pick_server(LoadBalanceAlogrithm lba){	
	if(lba == DistributedInProportion)	pick_server_proportion();
	else if(lba == DistributedInFastest)	pick_server_fastest();
	else pick_server_round();	
}
void LoadBalancer::pick_server_round(){
	if(server_support_count <= 0){	choosen_server = -1;	return;}
	int last;
	if(choosen_server == -1)	last = server_support_count-1;
	else	last = choosen_server;
	do {
		++choosen_server;
		choosen_server %= server_support_count;
		if(servers[choosen_server].condition == GOOD)	return;
	} while (choosen_server != last);
	choosen_server = -1;		//没有可用的服务器
}
void LoadBalancer::pick_server_proportion(){
	int total_load_weight = 0;
	for(int i = 0; i < server_support_count; i++){
		if(servers[i].condition == GOOD){
			total_load_weight += servers[i].load_weight;
		}
	}
	if(total_load_weight == 0) {//没有可用的服务器
		choosen_server = -1;
		return;
	}else{
		double rand_pro = (double)rand()/RAND_MAX;   //随机生成一个[0,1]之间的实数
		int accumulate_load_weight = 0;
		for(int j = 0; j < server_support_count; j++){
			if(servers[j].condition == GOOD){
				accumulate_load_weight += servers[j].load_weight;
				double p = (double)accumulate_load_weight/total_load_weight;
				if(rand_pro < p){
					choosen_server = j;
					return;
				}
				else continue;
			}
		}
	}	
}
void LoadBalancer::pick_server_fastest(){
	long min_reply_time = -1;
	for(int i = 0; i < server_support_count; i++){
		if(servers[i].condition == GOOD && (min_reply_time == -1 || servers[i].reply_time < min_reply_time)){
			min_reply_time = servers[i].reply_time;
			choosen_server = i;
		}
	}
	if(min_reply_time == -1)	choosen_server = -1;
}

void LoadBalancer::printDebugInformation_msg(string &s,t_msg &msg){
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

