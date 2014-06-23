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

	/*添加一个自身的订阅*/
	int Subscribe(TopicID topic , char* pAddr , unsigned short port);
	
	int Unsubscribe(TopicID topic);

	/*统计接收到数据的次数*/
	int Receive(TopicID topic);
	
	//Topic---IP&Port 映射结构(保存确定建立的连接)
	struct TopicIP** tip_;
	//实际的tip_的尺寸
	int tipSize;
};
#endif