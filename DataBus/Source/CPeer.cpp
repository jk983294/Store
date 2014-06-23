#include "../include/DataBus.h"

/************************************************************************/
/*                                                                      */
/************************************************************************/
CPeer::CPeer(SOCKET s,CReceiver* receiver,CSender* sender,char* peerAddr,u_short peerport)
{
	s_=s;
	receiver_ = receiver;
	sender_ = sender;
	if (peerAddr!=NULL)
	{
		strcpy(pAddr,peerAddr);
	}	
	port = peerport;
	for (int i=0;i<MAX_TOPIC_SIZE;i++)
	{
		subscribe_topicID[i]=-1;
	}
	valid_ = 1;
	MUTEX_INIT(sub_topic_id_);
}
CPeer::~CPeer()
{
	if(sender_)
		delete sender_;
	sender_ = 0;
	
	if (receiver_)
		delete receiver_;
	receiver_ = 0;
	MUTEX_DESTROY(sub_topic_id_);

}
int CPeer::insert_Subscribe_TopicID(TopicID topic)
{
	int index = 0;
	MUTEX_LOCK(sub_topic_id_);
	for (index=0;index<MAX_TOPIC_SIZE;index++)
	{
		if (subscribe_topicID[index] == topic)//已经有
		{
			MUTEX_UNLOCK(sub_topic_id_);
			return 1;
		}
	}
	for (index=0;index<MAX_TOPIC_SIZE;index++)//第一遍没有找到
	{
		if (subscribe_topicID[index] == -1){
			subscribe_topicID[index] = topic;
			MUTEX_UNLOCK(sub_topic_id_);
			return 1;
		}
	}
	MUTEX_UNLOCK(sub_topic_id_);
	return -1;//topic 满了
}

int CPeer::delete_Subscribe_TopicID(TopicID topic)
{
	int index = 0;
	MUTEX_LOCK(sub_topic_id_);
	for (index=0;index<MAX_TOPIC_SIZE;index++)
	{
		if (subscribe_topicID[index] == topic)//已经有
		{
			subscribe_topicID[index] = -1;
			MUTEX_UNLOCK(sub_topic_id_);
			return 1;
		}
	}
	MUTEX_UNLOCK(sub_topic_id_);
	return 1;//没找到  就不用显示退订
}
int CPeer::find_Subscribe_TopicID(TopicID topic)
{
	int index = 0;
	MUTEX_LOCK(sub_topic_id_);
	for (index=0;index<MAX_TOPIC_SIZE;index++)
	{
		if (subscribe_topicID[index] == topic)//已经有
		{
			MUTEX_UNLOCK(sub_topic_id_);
			return 1;
		}
	}
	MUTEX_UNLOCK(sub_topic_id_);
	return -1;
}

/**************************************************	
方法名称：sendData
输入参数：
buffer：输出缓冲区。
len：长度。
输出参数：无。
返回值： 
执行成功返回1，否则返回-1。
函数说明：此函数发送原始数据。
**************************************************/
int CPeer::sendData(char *buffer , int len )
{
	//MUTEX_LOCK(sendLock_);		
	int tmp=(sender_->Send(NULL,buffer , len));
	//MUTEX_UNLOCK(sendLock_);	
	return tmp;	
}

int CPeer::receive(TopicID& topic, char*& data , int& length){
	int tmp=(receiver_->Receive(topic,data,length));	
	return tmp;	
}
bool CPeer::compareAddrPort(char* peerAddr,u_short peerport){
	if ((strcmp(peerAddr,pAddr)==0) && (peerport==port))
		return true;
	else return false;
}