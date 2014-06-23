#ifndef CCONNECTOR_H
#define CCONNECTOR_H 

class CConnector
{
public:
	CConnector(){};
	~CConnector(){};
	virtual SOCKET Connect(short type,const char* ipaddr,unsigned short portNum)=0;
	virtual SOCKET DisConnect(const char* ipaddr,unsigned short portNum)=0;

};
#endif