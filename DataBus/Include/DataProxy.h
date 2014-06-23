#pragma once

class APPENCAPSULATEDLL_EXPORTS CDataProxy
{
public:
	CDataProxy(void);
	~CDataProxy(void);

	int Initialize();
	int Publish(CTopic topic);
	int UnPublish(CTopic topic);
	int UnPublish(TopicID topic);
	int Subscribe(CTopic topic);
	int UnSubscribe(CTopic topic);
	int UnSubscribe(TopicID topic);
	int Send(TopicID topic , CData data);

private:
	

};
