#ifndef CSENDINGARRAY_H
#define CSENDINGARRAY_H

class CSendingArray :public CDataArray{
public :
	CSendingArray();
	~CSendingArray ();

	
	/************************************************************************/
	/* ������������ķ�������ʱ��CSendingArray����һ���µĶ��У�			*/
	/*	һ���������������													*/
	/*	һ���������������ɹ�����1�����򷵻ش������						*/                                                                    
	/************************************************************************/
	int AddTopic(TopicID topic);

	int RemoveTopic(TopicID topic);

	/*�ϲ�Ӧ�õ��øýӿڽ���Ҫ���͵����ݷ��뻺�棬���ͷ��ź���*/
	int Send(TopicID topic, char* data , int length);

	/************************************************************************/
	/* CDataFlow���øýӿڻ�ȡ��Ҫ���͵�����,���������ݵ�topic                                                                     */
	/************************************************************************/
	int Receive(TopicID& topic, char*& data , int& length);

	
private :
	TopicArray* m_data[MAX_ARRAY_LENGTH];
	TopicID m_topics[MAX_TOPIC_SIZE];
	SEMA m_sema;
	MUTEX m_sendingarraylock;
	int m_count;

private:
	int TopicExisted(TopicID);
	
};

#endif