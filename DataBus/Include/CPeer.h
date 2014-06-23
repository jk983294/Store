#ifndef CPEER_H
#define CPEER_H

class CPeer
{
public:
	~CPeer();
	CPeer(SOCKET s,CReceiver* receiver,CSender* sender,char* peerAddr,u_short peerport);
	
	int sendData(char *buffer, int len );

	int receive(TopicID& topic, char*& data , int& length);

	int isvalid();

	bool compareAddrPort(char* peerAddr,u_short peerport);

	int insert_Subscribe_TopicID(TopicID topic);

	int delete_Subscribe_TopicID(TopicID topic);

	int find_Subscribe_TopicID(TopicID topic); 

	SOCKET s_;
	CReceiver* receiver_;
	CSender* sender_;
	char pAddr[64]; 
	u_short port;
	MUTEX sub_topic_id_;
	TopicID subscribe_topicID[MAX_TOPIC_SIZE];
	int reading_;
	time_t time_;
	int valid_;
};
#endif