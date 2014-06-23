#include "../include/DataBus.h"

CSendingArray::CSendingArray(){
	SEMA_INIT(m_sema,0,MAX_ARRAY_LENGTH);
	MUTEX_INIT(m_sendingarraylock);
	u_short index = 0;
	for(index=0;index<MAX_TOPIC_SIZE;index++){
		m_topics[index]=-1;
	}
	for(index=0;index<MAX_ARRAY_LENGTH;index++){
		m_data[index]=0;
	}
	m_count = 0;
}

CSendingArray::~CSendingArray()
{
	u_short index = 0;	
	for(index=0;index<MAX_ARRAY_LENGTH;index++)
	{
		if(m_data[index]!=0)
		{
			if (NULL!=m_data[index]->buf_)
			{
				free( m_data[index]->buf_);
			}
			delete m_data[index];
		}
	}
	SEMA_DESTROY(m_sema);
	MUTEX_DESTROY(m_sendingarraylock);
}
/*加入一个新的待发布的主题队列,已经有了返回0*/
int CSendingArray::AddTopic(TopicID topic){
	printf("Sending Array AddTopic(%d)\n",topic);
	int ret = -1;
	u_short index = 0;
	MUTEX_LOCK(m_sendingarraylock);
	for(index =0;index<MAX_TOPIC_SIZE;index++)
	{
		if(m_topics[index] == topic)
		{
			ret = 1;
			break;
		}
	}
	if(ret == -1)
	{
		for(index =0;index<MAX_TOPIC_SIZE;index++)
		{
			if(m_topics[index] == -1)
			{
				m_topics[index] = topic;
				ret = 1;
				break;
			}
		}
	}
	MUTEX_UNLOCK(m_sendingarraylock);
	return ret;
}

int CSendingArray::RemoveTopic(TopicID topic)
{
	printf("Sending Array RemoveTopic(%d)\n",topic);
	u_short index = 0;
	MUTEX_LOCK(m_sendingarraylock);	
	for(index =0;index<MAX_TOPIC_SIZE;index++)
	{
		if(m_topics[index] == topic)
		{
			m_topics[index] = -1;
			break;
		}
	}
	for(index = 0;index<MAX_ARRAY_LENGTH;index++)
	{
		if(m_data[index]!=0)
		{
			if(m_data[index]->topic_ == topic)
			{
				if(m_data[index]->buf_!=0)
					free(m_data[index]->buf_);
				delete m_data[index];
				m_data[index] = 0;
				m_count-=1;
				DWORD dw=SEMA_WAIT_TIME(m_sema,100);
				if(dw==WAIT_TIMEOUT)
				{
					printf("Something wrong when RemoveTopic\n");
				}
			}
		}
	}
	MUTEX_UNLOCK(m_sendingarraylock);
	return -1;
}

int CSendingArray::Send(TopicID topic,char* buf,int length)
{
	int postsema = 0;
	MUTEX_LOCK(m_sendingarraylock);
	if(TopicExisted(topic) == 1 && m_count < MAX_ARRAY_LENGTH)
	{
		u_short index = 0;
		for(index = 0;index<MAX_ARRAY_LENGTH;index++)
		{
			if(m_data[index]==0)
			{
				m_data[index] = new struct TopicArray;
				m_data[index]->topic_ = topic;
				m_data[index]->len_ = length;
				m_data[index]->buf_ = (char*)malloc(length);
				memcpy((void*)(m_data[index]->buf_) , (void*)buf , length);
				m_count+=1;
				postsema = 1;
				break;
			}
		}
	}
	MUTEX_UNLOCK(m_sendingarraylock);
	if(postsema)
		SEMA_POST(m_sema);
	return length;
}

/*从队列中读取数据，成功返回topic号，参数数组buf最好大于DEFAULT_BUF_SIZE*/
int CSendingArray::Receive(TopicID& topic, char*& buf , int& length){
	int ret=-1;

	DWORD dw=SEMA_WAIT_TIME(m_sema,1000*MULTICAST_PERIOD);
	
	if(dw==WAIT_TIMEOUT){
		return 0;
	}
	MUTEX_LOCK(m_sendingarraylock);
	int index = 0;
	for(index = 0;index<MAX_ARRAY_LENGTH;index++)
	{
		if(m_data[index]!=0 )
		{
			topic = m_data[index]->topic_;
			length = m_data[index]->len_;
			buf = (char*)malloc(length);
			memcpy((void*)buf , (void*)(m_data[index]->buf_) , length);
			free(m_data[index]->buf_);
			delete m_data[index];
			m_data[index] = 0;
			m_count-=1;
			ret = 1;
			break;
		}
	}
	MUTEX_UNLOCK(m_sendingarraylock);
	return ret;
}

int CSendingArray::TopicExisted(TopicID topic)
{
	int ret = -1;
	for(u_short i=0;i<MAX_TOPIC_SIZE;i++)
	{
		if(m_topics[i] == topic)
		{
			ret = 1;
			break;
		}
	}
	return ret;
}
