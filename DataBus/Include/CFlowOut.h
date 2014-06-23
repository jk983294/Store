#ifndef CFLOWOUT_H
#define CFLOWOUT_H

struct TopicCount{
	TopicID topic_;
	bool localSub;
	int sendCount_; //���ʹ���
	int subCount_; //��������
};

class CFlowOut{
public:
	CFlowOut();
	~CFlowOut();
	
	/*	���һ��������������*/
	int Publish(TopicID topic);
	
	/*  ��������������� */
	int Unpublish(TopicID topic);

	/*ͳ��topic�ı����Ĵ���+1*/
	int Subscribe(TopicID topic);
	/*ͳ��topic�ı����Ĵ���-1*/
	int UnSubscribe(TopicID topic);

	int LocalSubscribe(TopicID topic);
	bool isLocalSubscribed(TopicID topic);

	/*���ⷢ��һ������ʱ��¼*/
	int Send(TopicID topic);
	
	/*�ж�Topic�Ƿ������tc��*/
	bool TopicExist(TopicID topic);
	/*
	��װһ���鲥����
	*/
	void GetPublish(TopicID topics[]);
	
	TopicCount* tc_[MAX_TOPIC_SIZE];
	
};
#endif