#ifndef CDATACHANNEL_H
#define CDATACHANNEL_H 

class CDataChannel: public CThread
{
public:
	CDataChannel();
	~CDataChannel();

	void SetDomainID(domain_id domainid) {m_domainid = domainid;}

	int addselectsocket();
	
	int delPeerByIndex(int i);


	//与CDataFlow建立连接
	int Assemble(CDataFlow* cdf1){cdf=cdf1;return 1;};



	void Assemble_Connector(CConnector* conn){connector = conn;};

	/*CDataFlow调用该接口向网络发送数据
	两个输入参数：需要发送的数据和主题ID
	一个输出参数：成功则返回1，否则返回错误代码；
	*/
	int Send(int topic, char* data , int length);

	/************************************************************************/
	/* Multicast                                                            */
	/************************************************************************/
	int Publish(int topicID);

	int publishFlowIn(TopicID topic,u_short reference);

	int publishFlowOut(TopicID topic,u_short reference);

	int UnPublish(TopicID topic);
	//用来中断select 成功返回 1
	int Interrupt_select();

	int Subscribe(int topic, char* pAddr, u_short port);

	int UnSubscribe(int topic);

	/*将生成的CPeer加入
	两个输入参数：对端IP和CPeer实例；
	一个输出参数：一个输出参数：成功则返回1，否则返回错误代码；
	*/
	int Register(char* pAddr , unsigned short port ,CPeer* pPeer);

	/*将已有的CPeer剥离
	两个输入参数：对端IP和CPeer实例引用；
	一个输出参数：一个输出参数：成功则返回1，否则返回错误代码；
	*/
	int UnRegister(CPeer* pPeer);

	int FindPeer(char* pAddr , unsigned short port);

	int TestPeers();

	void setMulticastPeer(CPeer* mp){multicast_peer = mp;};

	void setInterruptSender(CPeer* is){interrupt_recver = is;};//为了datachannel的select机制不需要sleep作一个互换

	void setInterruptRecver(CPeer* ir){interrupt_sender  = ir;};

	void setConnector(CConnector* conn){connector = conn;};

	//void setAcceptor(CConnector* acceptor){pacceptor = acceptor;};

	void setMulticastparameters(u_short port,char* paddr){
		multicast_host_port = port;
		strcpy(multicast_host_addr,paddr);
	};

	virtual void Run();

	//int subscribe(int topicID,char *peerIP,char* port);
private:
	int checksubscribe(int topic, char* pAddr, u_short port);
	int insert_topic_peer_map(int topic,int index);
	int un_insert_topic_peer_map(int topic,int index);
 	//int delete_peer_in_topicmap_by_topic(int index);
 	//int delete_topic_in_topicmap(TopicID topic);
private:
	
	//CPeer实例列表
	CPeer* m_peers[MAXPEERNUM];
	CPeer* multicast_peer;
	CPeer* interrupt_sender;
	CPeer* interrupt_recver;

	//mutex for add/delete peer
	MUTEX peer_lock_;

	CConnector* connector;
	CDataFlow* cdf;

	domain_id m_domainid;

	/*主题到CPeer的映射，每行的第一个数字是主题的编号，
	其余数字为需要发送的CPeer的下标
	*/
	int m_topicmapping[TOPIC_MAXNUM][PEER_MAXNUM_FOR_EACH_TOPIC];
public:
	u_short multicast_host_port;
	char multicast_host_addr[64];
	char* tcpheader_;
	char* udpheader_;
	CSocket_utility* csu;
	//CAcceptor* pacceptor;
};

#endif 