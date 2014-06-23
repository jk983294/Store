#ifndef NETMONITOR_HH
#define NETMONITOR_HH

class CNetMonitor : public CThread
{
public:
	CNetMonitor() {}
	~CNetMonitor() {}
	void Assemble(CDataChannel* datachannel) { m_datachannel = datachannel; }
	void Run();

private:
	CDataChannel* m_datachannel;
};

#endif