#pragma once

class DLL_API CThread
{
public:
	CThread(void);
	~CThread(void);
	void Start();
	void Stop();
	virtual void Run() = 0;
private:
	DWORD m_ThreadId;
	HANDLE m_ThreadHandle;
};
