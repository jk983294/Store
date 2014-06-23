#ifndef CFLOWIN_H
#define CFLOWIN_H

struct TopicIP 
{
	TopicID topic_;
	char ip[16];
	unsigned short port;
	int reference;
};

class CFlowIn{
public:
	CFlowIn();
	~CFlowIn();
	

	int Publish(TopicID topic );

	/*���һ������Ķ���*/
	int Subscribe(TopicID topic , char* pAddr , unsigned short port);
	
	int Unsubscribe(TopicID topic);

	/*ͳ�ƽ��յ����ݵĴ���*/
	int Receive(TopicID topic);
	
	//Topic---IP&Port ӳ��ṹ(����ȷ������������)
	struct TopicIP** tip_;
	//ʵ�ʵ�tip_�ĳߴ�
	int tipSize;
};
#endif