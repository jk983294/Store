#ifndef CACCEPTOR_H
#define CACCEPTOR_H

class CAcceptor:public CThread
{
public:
	CAcceptor(){};
	~CAcceptor(){};
	
	virtual void SetAddr(const char* ipaddr,unsigned short portNum)=0;
	virtual void Run()=0;
};
#endif