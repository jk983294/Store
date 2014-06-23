#include "../include/DataBus.h"

CDataReader::CDataReader(void)
{
	m_topic = -1;
	m_databuf = 0;
	m_subscriber = 0;
}

CDataReader::~CDataReader(void)
{
	if(m_databuf)
		delete m_databuf;
}

void CDataReader::SetSubscriber(CSubscriber* pSubscriber)
{
	m_subscriber = pSubscriber;
}
TopicID CDataReader::Receive(CData& data)
{
	return m_subscriber->Read(data);
}
