#ifndef CNETWORKOPERATOR_H
#define CNETWORKOPERATOR_H

class CNetworkOperator
{
public:
	
	CNetworkOperator();
	~CNetworkOperator();
	
	int Initialize(domain_id domainid = 0);
	
	void Destroy();

	//��CDataFlow�������ӣ��ڲ����CDataChannel��CDataFlow�����ӣ�
	int Assemble(CDataFlow* cdf,CDataArray* cSendingArray,CDataArray* cReceivingArray);

	//�����CDataFlow֮��Ĺ�����
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

