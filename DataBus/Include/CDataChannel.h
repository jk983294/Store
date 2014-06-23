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


	//��CDataFlow��������
	int Assemble(CDataFlow* cdf1){cdf=cdf1;return 1;};



	void Assemble_Connector(CConnector* conn){connector = conn;};

	/*CDataFlow���øýӿ������緢������
	���������������Ҫ���͵����ݺ�����ID
	һ������������ɹ��򷵻�1�����򷵻ش�����룻
	*/
	int Send(int topic, char* data , int length);

	/************************************************************************/
	/* Multicast                                                            */
	/************************************************************************/
	int Publish(int topicID);

	int publishFlowIn(TopicID topic,u_short reference);

	int publishFlowOut(TopicID topic,u_short reference);

	int UnPublish(TopicID topic);
	//�����ж�select �ɹ����� 1
	int Interrupt_select();

	int Subscribe(int topic, char* pAddr, u_short port);

	int UnSubscribe(int topic);

	/*�����ɵ�CPeer����
	��������������Զ�IP��CPeerʵ����
	һ�����������һ������������ɹ��򷵻�1�����򷵻ش�����룻
	*/
	int Register(char* pAddr , unsigned short port ,CPeer* pPeer);

	/*�����е�CPeer����
	��������������Զ�IP��CPeerʵ�����ã�
	һ�����������һ������������ɹ��򷵻�1�����򷵻ش�����룻
	*/
	int UnRegister(CPeer* pPeer);

	int FindPeer(char* pAddr , unsigned short port);

	int TestPeers();

	void setMulticastPeer(CPeer* mp){multicast_peer = mp;};

	void setInterruptSender(CPeer* is){interrupt_recver = is;};//Ϊ��datachannel��select���Ʋ���Ҫsleep��һ������

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
	
	//CPeerʵ���б�
	CPeer* m_peers[MAXPEERNUM];
	CPeer* multicast_peer;
	CPeer* interrupt_sender;
	CPeer* interrupt_recver;

	//mutex for add/delete peer
	MUTEX peer_lock_;

	CConnector* connector;
	CDataFlow* cdf;

	domain_id m_domainid;

	/*���⵽CPeer��ӳ�䣬ÿ�еĵ�һ������������ı�ţ�
	��������Ϊ��Ҫ���͵�CPeer���±�
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