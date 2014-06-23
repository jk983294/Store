#include "../include/DataBus.h"

CReceivingArray::CReceivingArray(){
	SEMA_INIT(m_sema,0,MAX_ARRAY_LENGTH);
	MUTEX_INIT(m_lock);
	m_count = 0;
	for(u_short i=0;i<MAX_ARRAY_LENGTH;i++)
	{
		m_topics[i]=0;
	}
}

CReceivingArray::~CReceivingArray()
{
	for(u_short i=0;i<MAX_ARRAY_LENGTH;i++)
	{
		if(m_topics[i]!=0)
		{
			if (NULL!=m_topics[i]->buf_)
			{
				free( m_topics[i]->buf_);
			}
			delete m_topics[i];
		}
	}
	SEMA_DESTROY(m_sema);
	MUTEX_DESTROY(m_lock);
}


int CReceivingArray::AddTopic(TopicID topic){
	return 1;
}

int CReceivingArray::RemoveTopic(TopicID topic){
	return 1;
}
/*�ϲ�Ӧ�õ��øû�ȡ���������ϵ����ݣ�ͨ��CPrioSettingTestȷ������*/
int CReceivingArray::Receive(TopicID& topic, char*& data , int& length)
{
	int ret=-1;
	DWORD dw=SEMA_WAIT(m_sema);
	MUTEX_LOCK(m_lock);
	for(u_short i=0;i<MAX_ARRAY_LENGTH;i++)
	{
		if(m_topics[i]!=0 )
		{
			//������ȡ����
			data = (char*)malloc(m_topics[i]->len_);
			memcpy((void*)data , (void*)(m_topics[i]->buf_) , m_topics[i]->len_);
			length = m_topics[i]->len_;
			topic = m_topics[i]->topic_;
			ret=1;

			free(m_topics[i]->buf_);
			delete m_topics[i];
			m_topics[i] = 0;
			break;
		}
	}
	MUTEX_UNLOCK(m_lock);
	m_count-=1;
	return ret;
}
/*CDataFlow���øýӿڽ������ϵ����ݴ��뻺��*/
int CReceivingArray::Send(TopicID topic, char* data , int length)
{
	int len=-1;
	MUTEX_LOCK(m_lock);
	for(u_short i=0;i<MAX_ARRAY_LENGTH;i++)
	{
		if(m_topics[i] == 0)
		{
			m_topics[i] = new struct TopicArray;
			m_topics[i]->topic_ = topic;
			m_topics[i]->len_ = length;
			m_topics[i]->buf_ = (char*)malloc(length);
			memcpy((void*)(m_topics[i]->buf_) , (void*)data , length);
			break;
		}
	}
	MUTEX_UNLOCK(m_lock);
	SEMA_POST(m_sema);
	m_count+=1;
	return len;
}