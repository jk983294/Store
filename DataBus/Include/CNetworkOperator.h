#ifndef CNETWORKOPERATOR_H
#define CNETWORKOPERATOR_H

class CNetworkOperator
{
public:
	
	CNetworkOperator();
	~CNetworkOperator();
	
	int Initialize(domain_id domainid = 0);
	
	void Destroy();

	//与CDataFlow建立连接，内部完成CDataChannel与CDataFlow的连接；
	int Assemble(CDataFlow* cdf,CDataArray* cSendingArray,CDataArray* cReceivingArray);

	//拆分与CDataFlow之间的关联；
	void Split();
private:
	int interrupt_peer_init();
	int multicast_peer_init();
	CDataChannel* datachannel;
	CConnector* connector;
	CAcceptor* acceptor;
	CNetMonitor* m_netmonitor;
	domain_id m_domainid;
};
#endif

