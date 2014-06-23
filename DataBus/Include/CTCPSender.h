#ifndef CTCPSENDER_H
#define CTCPSENDER_H


class CTCPSender:public CSender{
public:
	CTCPSender(SOCKET s);

	~CTCPSender();	
	//·¢ËÍÊý¾Ý
	int Send(int topicID,char* buffer , int len);
private:

	SOCKET s_;


};
#endif