#ifndef CSENDINGARRAY_H
#define CSENDINGARRAY_H

class CSendingArray :public CDataArray{
public :
	CSendingArray();
	~CSendingArray ();

	
	/************************************************************************/
	/* 接收来自组件的发布请求时，CSendingArray建立一个新的队列；			*/
	/*	一个输入参数：主题													*/
	/*	一个输出参数：如果成功返回1，否则返回错误代码						*/                                                                    
	/************************************************************************/
	int AddTopic(TopicID topic);

	int RemoveTopic(TopicID topic);

	/*上层应用调用该接口将需要发送的数据放入缓存，并释放信号量*/
	int Send(TopicID topic, char* data , int length);

	/************************************************************************/
	/* CDataFlow调用该接口获取需要发送的数据,返回有数据的topic                                                                     */
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