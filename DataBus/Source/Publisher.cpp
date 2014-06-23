#include "../include/DataBus.h"

CPublisher::CPublisher(void)
{
	m_dataflow = 0;
	m_sendingarray = 0;
	m_datawriter = 0;
}

CPublisher::~CPublisher(void)
{
	if(m_datawriter)
		delete m_datawriter;
}

void CPublisher::Assemble(CDataFlow* pDataFlow , CSendingArray* pSendingArray)
{
	m_dataflow = pDataFlow;
	m_sendingarray = pSendingArray;
}

int CPublisher::Publish(TopicID topic)
{
	if(m_dataflow)
	{
		return m_dataflow->Publish(topic);
	}
	else
	{
		return -1;
	}
}

int CPublisher::UnPublish(TopicID topic)
{
	if(m_dataflow)
	{
		return m_dataflow->UnPublish(topic);
	}
	else
	{
		return -1;
	}
}

int CPublisher::Send(CData data)
{
	int tmplength = 0;
	void* tmpbuf = 0;
	tmplength = data.Encode(tmpbuf);
	int rst = 0;
	rst = m_sendingarray->Send(data.GetTopic() , (char*)tmpbuf , tmplength);
	free(tmpbuf);
	return rst;
}

CDataWriter* CPublisher::GetWriter()
{
	if(m_datawriter)
		return m_datawriter;
	else
	{
		m_datawriter = new CDataWriter;
		m_datawriter->SetPublisher(this);
		return m_datawriter;
	}
}
