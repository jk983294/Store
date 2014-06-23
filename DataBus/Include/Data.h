#pragma once

class DLL_API CData
{
public:
	CData(void);
	CData(CData& data);
	~CData(void);
	
	void SetTopicID(TopicID topic);
	TopicID GetTopic();
	int Length(int length);
	int Length();

	CData& operator=(CData data);
	CAny& operator [](int index);
	int Encode(void*& buf);
	int Decode(void* buf);
	int DecodeCompState(void* buf);
private:
	CAny* m_data[MAX_CDATA_LENGTH];
	int m_length;
	TopicID m_topic;
};
