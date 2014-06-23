
class CTCPAcceptor:public CAcceptor{
public:
	CTCPAcceptor(CDataChannel* datachannel);
	~CTCPAcceptor();
	
	void SetAddr(const char* ipaddr,unsigned short portNum);
	void Run();
	bool SelfInterupt();
private:
	
	CDataChannel* datachannel_;
	
	SOCKET s_;

	bool m_dbc_tcpacceptor_selfinterupt;

};