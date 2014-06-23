#include "DataBus.h"

void CNetMonitor::Run()
{
	while(1)
	{
		int ret = m_datachannel->TestPeers();
		if(!ret)
		{
			m_datachannel->Stop();
			m_datachannel->Start();
		}
		Sleep(1000);
	}
	
}