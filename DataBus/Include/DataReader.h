#pragma once

class DLL_API CDataReader
{
public:
	CDataReader(void);
	~CDataReader(void);

	void SetSubscriber(CSubscriber* pSubscriber);
	TopicID Receive(CData& data);
private:
	TopicID m_topic;
	CData* m_databuf;
	CSubscriber* m_subscriber;
};
