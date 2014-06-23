#ifndef CRECEIVINGARRAY_H
#define CRECEIVINGARRAY_H


class CReceivingArray :public CDataArray{
public :
	CReceivingArray();
	~CReceivingArray();

	int AddTopic(TopicID topic);
	
	int RemoveTopic(TopicID topic);
	
	/**/
	int Send(TopicID topic, char* data , int length);
	
	/**/
	int Receive(TopicID& topic, char*& data , int& length);

private :
	TopicArray* m_topics[MAX_ARRAY_LENGTH];
	SEMA m_sema;
	MUTEX m_lock;
	int m_count;
};

#endif