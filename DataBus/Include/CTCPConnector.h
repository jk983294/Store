

class CTCPConnector: public CConnector
{
public:
	CTCPConnector(CDataChannel* datachannel);
	~CTCPConnector();
	 SOCKET Connect(short type,const char* ipaddr,unsigned short portNum);
	 SOCKET DisConnect(const char* ipaddr,unsigned short portNum);
	 //SOCKET interrupt_connect();
private:
	
	CDataChannel* datachannel_;	
	SOCKET s_;
	char* tcpheader_;
};