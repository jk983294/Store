#include "CSender.h"

class CUDPSender:public CSender{
public:
	CUDPSender(SOCKET s);

	~CUDPSender();	
	//·¢ËÍÊı¾İ
	int Send(int topicID,char* buffer , int len);
private:
	
	SOCKET s_;

	struct sockaddr_in dest_addr;
	
};