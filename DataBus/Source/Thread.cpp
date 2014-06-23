#include "../include/DataBus.h"

DWORD WINAPI alpStartAddress(LPVOID param)
{
	CThread * thread=(CThread *) param;
	thread->Run();
	return 0;
}

CThread::CThread(void)
{
}

CThread::~CThread(void)
{
}

void CThread::Start()
{
	m_ThreadHandle = ::CreateThread(NULL, 0, alpStartAddress, this, 0, &m_ThreadId);
}

void CThread::Stop()
{
	::TerminateThread(m_ThreadHandle , 0);
}
