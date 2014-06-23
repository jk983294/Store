
/***************************************************
�ļ����� LoadBalancer.cpp
˵����	���ؾ��������Դ�ļ����������ĺ���
���ߣ�   ����
ʱ�䣺	2013-08-1
�汾��	����
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
	logger = new Logger(log_path);   //��־���ʼ��
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
	//��ʼ��socket
	if (! SocketUtil::create_udp_socket(client_socket))
		logger->log_error_time_ln("����client socket����");
	if(! SocketUtil::udp_bind(client_socket,client_udp_port))
		logger->log_error_time_ln("client socket�󶨴���");
	if (! SocketUtil::create_udp_socket(server_socket))
		logger->log_error_time_ln("����server socket����");
	if(! SocketUtil::udp_bind(server_socket,server_udp_port))
		logger->log_error_time_ln("server socket�󶨴���");
	logger->log_time_ln("��ʼ��client_socket��server_socket���");
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
					if(isdebug)	printDebugInformation("���ؾ����㷨Ϊ���������ַ��ĸ��ؾ����㷨��");
					logger->log_time_ln("���ؾ����㷨Ϊ���������ַ��ĸ��ؾ����㷨��");	
				}else if(type == 1){
					load_balance_algorithm = DistributedInFastest;
					if(isdebug)	printDebugInformation("���ؾ����㷨Ϊ�����������Ӧ�ĸ��ؾ����㷨��");
					logger->log_time_ln("���ؾ����㷨Ϊ�����������Ӧ�ĸ��ؾ����㷨��");
				}else{
					load_balance_algorithm = round_alogrithm;
					if(isdebug)	printDebugInformation("���ؾ����㷨Ϊ����ת�㷨����");
					logger->log_time_ln("���ؾ����㷨Ϊ����ת�㷨��");	
				}
			}else if(key == "session_keep_method"){
				ss.clear(); ss<<value; ss>>type;
				if(type == 0){
				    session_keep_method = SrcIdBased;
					if(isdebug)	printDebugInformation("�Ự���֣�����src_id�ĻỰ���֣�");
					logger->log_time_ln("�Ự���֣�����src_id�ĻỰ���֣�");
				}else if(type == 1){
					session_keep_method = UsrIdBased;
					if(isdebug)	printDebugInformation("�Ự���֣�����usr_id�ĻỰ���֣�");
					logger->log_time_ln("�Ự���֣�����usr_id�ĻỰ���֣�");
				}else{
					session_keep_method = invalid_session_keep_method;
					if(isdebug)	printDebugInformation("��ʹ���κλỰ���ֻ��ƣ�");
					logger->log_time_ln("��ʹ���κλỰ���ֻ��ƣ�");
				}
			}
		}
		logger->log_time_ln("��ȡ�����ļ��ɹ�");
	}else{
		printDebugInformation("��ȡ�����ļ�ʧ��");
		logger->log_error_time_ln("��ȡ�����ļ�ʧ��");
	}
}
void LoadBalancer::run(){
	pkt_thread.ThreadHandle = CreateThread(NULL, 0, StartPktThreadAddress, this, 0, &pkt_thread.ThreadID);
	send_heartbeat_thread.ThreadHandle = CreateThread(NULL, 0, StartSendHeartbeatThreadAddress, this, 0, &send_heartbeat_thread.ThreadID);
	detect_thread.ThreadHandle = CreateThread(NULL, 0, StartDetectThreadAddress, this, 0, &detect_thread.ThreadID);
	int choice;
	bool isEnd = false;
	cout<<"���ؾ���� ( id = "<<id<<" ) ����..."<<endl
		<<"��1 ��debug����"<<endl
		<<"��2 �ر�debug����"<<endl
		<<"��3 ��ʾͳ������"<<endl
		<<"��4 �رո��ؾ������"<<endl;
	while (cin>>choice){		
		switch(choice){
		case 1:	isdebug = true;	 logger->log_time_ln("��debug����");	break;
		case 2: isdebug = false; logger->log_time_ln("�ر�debug����");	break;
		case 3: print_statistics(); break;
		case 4: printDebugInformation("�رո��ؾ������...");	logger->log_time_ln("�رո��ؾ������...");	isEnd = true;	break;
		default: printDebugInformation("û�и�ָ��");	break;
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
	struct sockaddr_in from_,server_addr;			//�Է���Ϣ	
	server_addr.sin_family=AF_INET;    
	server_addr.sin_addr.s_addr=inet_addr(ip.c_str());
	int BUFFER_LEN = sizeof(t_msg),	ADDR_LEN = sizeof(from_);
	fd_set fdread; //ά��һ��SOCKET����
	while (true)
	{
		FD_ZERO(&fdread);
		FD_SET(client_socket,&fdread);		//����client_socket
		FD_SET(server_socket,&fdread);		//����server_socket
		ret = select(0, &fdread, NULL, NULL, NULL);
		//����client����������
		if (client_socket 	&&	(FD_ISSET(client_socket, &fdread))){
			if(recvfrom(client_socket, (char*)&msg, BUFFER_LEN, 0, (struct sockaddr *) &from_, (int *)&ADDR_LEN) < 0){
				logger->log_error_time_ln("��client�����մ���");
			}else{
				if (msg.dst_id == id){							
					if (msg.msg_type == MSG_TYPE_TIME_REQUST){	//ʱ��������	
						++ClientRecvCorrectCount;
						bool invaid_alogrithm;
						if(isdebug){
							ss<<"��ȷ���մ�Client�˷�����ʱ�������ģ�";
							content = ss.str();		ss.str("");
							printDebugInformation_NoDateMsg(content,msg);
						}
						clients_usrid_addr[msg.usr_id] = from_;					//�û������ڱ��socket�ϵ�½��ֱ�Ӹ���ԭ���ļ�¼
						clients_usrid_dstid[msg.usr_id]	= msg.src_id;				
						pick_server_session(msg);                      //ѡ������������ǻỰ���ֹ���
						if (choosen_server	<	0){
							ss<<"�Ҳ������������������";	content = ss.str();	ss.str(""); 
							logger->log_time_ln(content);
							if(isdebug)	printDebugInformation(content);
						}else{
							server_addr.sin_port = htons(servers[choosen_server].port);  
							msg.dst_id = servers[choosen_server].id;	msg.src_id = id;
							if(send_data_thread_safe(msg,BUFFER_LEN,server_addr,ADDR_LEN) < 0){
								ss<<"����ʱ�������ĸ�������  "<<msg.dst_id<<" ����  ";	content = ss.str();	ss.str(""); 
								if(isdebug) perror(content.c_str());
								logger->log_error_time_ln(content);
							}
							else{	//��ȷ����ʱ��������
								++ServerSendCount;
								ss<<"���Ϳͻ�( usr_id = "<<msg.usr_id<<" ) ��ʱ�������ĸ�������: "<<servers[choosen_server].id<<" �ɹ�";
								content = ss.str();	ss.str(""); 
								logger->log_time_ln(content);
								if(isdebug)	printDebugInformation_NoDateMsg(content,msg);
							}
						}
						
					}
					else{//δ������Ϣ���ͱ��Ĵ���
						++ClientRecvIncorrectCount;
						ss<<"�ӿͻ����յ�δ������Ϣ���͵ı���";	content = ss.str();	ss.str(""); 
						if(isdebug) printDebugInformation(content);
						logger->log_time_ln(content);
					}
				}else{	//dst_id != id
					++ClientRecvIncorrectCount;
					ss<<"�ӿͻ����յ�Ŀ���ַ�����ҵı���";	content = ss.str();	ss.str(""); 
					if(isdebug) printDebugInformation(content);
					logger->log_time_ln(content);
				}
			}
		}
		if (server_socket 	&&	(FD_ISSET(server_socket, &fdread))){
			if(recvfrom(server_socket, (char*)&msg, BUFFER_LEN, 0, (struct sockaddr *) &from_, (int *)&ADDR_LEN) < 0){
				logger->log_error_time_ln("�ӷ����������մ���");
			}else{
				if (msg.dst_id == id){						
					if (msg.msg_type == MSG_TYPE_TIME_REPLY){	//ʱ������ظ�����	
						++ServerRecvCorrectCount;	
						if(isdebug){
							ss<<"��ȷ���մӷ���˷�����ʱ���Ӧ���ģ�";
							content = ss.str(); ss.str("");
							printDebugInformation_msg(content,msg);
						}
						srcid = msg.src_id;
						msg.dst_id = clients_usrid_dstid[msg.usr_id];		msg.src_id = id;
						if(sendto(client_socket, (char*)&msg, BUFFER_LEN, 0, (struct sockaddr *) &clients_usrid_addr[msg.usr_id], ADDR_LEN) < 0){
							ss<<"���ͷ����� "<<srcid<<" ��ʱ��ظ����ĸ��ͻ��� ( usr_id = "<<msg.usr_id<<" ) ���� ";	content = ss.str();	ss.str(""); 
							if(isdebug) perror(content.c_str());
							logger->log_error_time_ln(content);
						}else{	//��ȷ����ʱ��ظ�����
							++ClientSendCount;
							ss<<"���ͷ����� "<<srcid<<" ��ʱ��ظ����ĸ��ͻ��� ( usr_id = "<<msg.usr_id<<" ) �ɹ�: ";	content = ss.str();	ss.str(""); 
							logger->log_time_ln(content);
							if(isdebug)	printDebugInformation_msg(content,msg);
						}
					}
					else if (msg.msg_type == MSG_TYPE_HEART_REPLY){	//��������Ӧ����
						++ServerRecvCorrectCount;
						//�鿴��ǰʱ�� ������Ӧʱ��
						time_t now = time(NULL);
						time_t send_time;
						char *data = msg.data;
						ss.clear(); ss << data; ss >> send_time; ss.str("");	        
						for(int i = 0; i < server_support_count; i++){
							if(servers[i].id == msg.src_id){							
								servers[i].reply_time = now - send_time;        //������Ӧʱ��
							    servers[i].send_heartbeat_no_accept_times = 0;  //�յ�����֮�󣬰������ղ����Ĵ�����0
								break;
							}	
						}
						if(isdebug){
							ss.clear();	ss<<"��ȷ���մӷ���� ( id = "<<msg.src_id<<" )������������Ӧ���ģ�";
							content = ss.str(); ss.str("");
							printDebugInformation_HeartBeatMsg(content,msg);
						}	
					}
					else{	//δ������Ϣ���ͱ��Ĵ���
						++ServerRecvIncorrectCount;
						ss<<"�ӷ��������յ�δ������Ϣ���͵ı���";	content = ss.str();	ss.str(""); 
						if(isdebug) printDebugInformation(content);
						logger->log_time_ln(content);
					}
				}else{	//dst_id != id
					++ServerRecvIncorrectCount;
					ss<<"�ӷ��������յ�Ŀ���ַ�����ҵı���";	content = ss.str();	ss.str(""); 
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
	struct sockaddr_in server_addr;			//�Է���Ϣ	
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
				 ss<<"�������������ĸ�������  "<<msg.dst_id<<" ����  ";	content = ss.str();	ss.str(""); 
				if(isdebug) perror(content.c_str());
				logger->log_error_time_ln(content);
				servers[i].condition = BAD;
			}
			else{	//��ȷ��������������
				servers[i].send_heartbeat_no_accept_times++;
				++ServerSendCount;
				ss<<"�������������ĸ������� "<<servers[i].id<<" �ɹ�: ";
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
		//����������״̬
		for(int i = 0; i < server_support_count; i++){
			//�������ɻ���õļ��
			if(servers[i].condition == BAD && servers[i].send_heartbeat_no_accept_times < 4) {
				servers[i].condition = GOOD;
				ss<<servers[i].id<<"�ŷ������ָ�������";
				content = ss.str();	ss.str(""); 
				if(isdebug)	printDebugInformation(content);
				logger->log_time_ln(content);
			}
			//�������ɺñ仵�ļ��
			if(servers[i].condition == GOOD && servers[i].send_heartbeat_no_accept_times > 4){
				servers[i].condition = BAD;
				ss<<servers[i].id<<"�ŷ�����ʧЧ��";
				content = ss.str();	ss.str(""); 
				if(isdebug)	printDebugInformation(content);
				logger->log_time_ln(content);
			}
		}
		//���ͻ��˻Ự�Ƿ�ʱ
		map<unsigned, struct session_keep_in_server>::iterator it; 
		if(session_keep_method == SrcIdBased){
			MUTEX_LOCK(mutex_session_modify);
			for(it = srcid_serverid.begin(); it != srcid_serverid.end(); ){
				if(timeUtil::getSecondlong()-(it->second).time >= 30){   //ɾ���Ự��Ŀ
					ss<<"src_idΪ "<<it->first<<" �ĻỰ��Ŀ��ɾ����";
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
				if(timeUtil::getSecondlong()-(it->second).time >= 30) {  //ɾ���Ự��Ŀ	
					ss<<"usr_idΪ "<<it->first<<" �ĻỰ��Ŀ��ɾ����";
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
		//�Ѿ����ڸûỰ��Ŀ
		if(srcid_serverid.count(msg.src_id) == 1 && servers[srcid_serverid[msg.src_id].server_id].condition == GOOD)    //���Ѵ��ڸûỰ�ҷ������Ǻõ�
		{
			choosen_server = srcid_serverid[msg.src_id].server_id;
			//�������»�Ծʱ��
			srcid_serverid[msg.src_id].time = timeUtil::getSecondlong();
		}else{
			pick_server(load_balance_algorithm);	//����ѡ���ĸ��ؾ����㷨ѡ��һ��������
			if (choosen_server	>=	0){
				//��ӻỰ��¼,���»Ự���ֵ�ʱ��
				srcid_serverid[msg.src_id].server_id = choosen_server;
				srcid_serverid[msg.src_id].time = timeUtil::getSecondlong();
			}
		}
	}else if(session_keep_method == UsrIdBased){
		//�Ѿ����ڸûỰ��Ŀ
		if(usrid_serverid.count(msg.usr_id) == 1 && servers[usrid_serverid[msg.usr_id].server_id].condition == GOOD)    //���Ѵ��ڸûỰ�ҷ������Ǻõ�
		{
			choosen_server = usrid_serverid[msg.usr_id].server_id;
			//�������»�Ծʱ��
			usrid_serverid[msg.usr_id].time = timeUtil::getSecondlong();
		}else{
			pick_server(load_balance_algorithm);	//����ѡ���ĸ��ؾ����㷨ѡ��һ��������
			if (choosen_server	>=	0){
				//��ӻỰ��¼,���»Ự���ֵ�ʱ��
				usrid_serverid[msg.usr_id].server_id = choosen_server;
				usrid_serverid[msg.usr_id].time = timeUtil::getSecondlong();
			}
		}
	}else	pick_server(load_balance_algorithm);	   //����ѡ���ĸ��ؾ����㷨ѡ��һ��������
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
	choosen_server = -1;		//û�п��õķ�����
}
void LoadBalancer::pick_server_proportion(){
	int total_load_weight = 0;
	for(int i = 0; i < server_support_count; i++){
		if(servers[i].condition == GOOD){
			total_load_weight += servers[i].load_weight;
		}
	}
	if(total_load_weight == 0) {//û�п��õķ�����
		choosen_server = -1;
		return;
	}else{
		double rand_pro = (double)rand()/RAND_MAX;   //�������һ��[0,1]֮���ʵ��
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
	ss<<"�յ��ͻ��˱�����: "<<ClientRecvCorrectCount+ClientRecvIncorrectCount<<" (�յ��ͻ�����ȷ������: "<<ClientRecvCorrectCount<<" �յ��ͻ��˴�������: "<<ClientRecvIncorrectCount
		<<") ���Ϳͻ��˱�����: "<<ClientSendCount<<"�յ�����˱�����: "<<ServerRecvCorrectCount+ServerRecvIncorrectCount<<" (�յ�����������ȷ������: "<<ServerRecvCorrectCount
		<<" �յ��������˴�������: "<<ServerRecvIncorrectCount<<") ���ͷ������˱�����: "<<ServerSendCount;
	string statistics = ss.str();
	logger->log_time_ln(statistics);	
	cout<<statistics<<endl;
	MUTEX_UNLOCK(mutex_print);
}

