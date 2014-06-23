#ifndef CFLOWOUT_H
#define CFLOWOUT_H

struct TopicCount{
	TopicID topic_;
	bool localSub;
	int sendCount_; //发送次数
	int subCount_; //订阅数量
};

class CFlowOut{
public:
	CFlowOut();
	~CFlowOut();
	
	/*	添加一个自身发布的主题*/
	int Publish(TopicID topic);
	
	/*  解除自身发布的主题 */
	int Unpublish(TopicID topic);

	/*统计topic的被订阅次数+1*/
	int Subscribe(TopicID topic);
	/*统计topic的被订阅次数-1*/
	int UnSubscribe(TopicID topic);

	int LocalSubscribe(TopicID topic);
	bool isLocalSubscribed(TopicID topic);

	/*对外发送一个数据时记录*/
	int Send(TopicID topic);
	
	/*判断Topic是否存在于tc种*/
	bool TopicExist(TopicID topic);
	/*
	组装一个组播报文
	*/
	void GetPublish(TopicID topics[]);
	
	TopicCount* tc_[MAX_TOPIC_SIZE];
	
};
#endif