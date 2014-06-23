#ifndef CDATAARRAY_H
#define CDATAARRAY_H

struct TopicArray{
	TopicID topic_;
	char* buf_;//���ݻ���
	int len_;//ʵ�����ݳ���
};
class CDataArray: public CSender, public CReceiver{
public:

	virtual int AddTopic(TopicID topic)=0;
	 
	virtual int RemoveTopic(TopicID topic)=0;

	virtual int Send(TopicID topic, char* data , int length)=0;
	
	virtual int Receive(TopicID& topic, char*& data , int& length)=0;
};
#endif