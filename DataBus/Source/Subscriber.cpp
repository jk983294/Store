#include "../include/DataBus.h"

CSubscriber::CSubscriber()
{
	m_dataflow = 0;
	m_receivingarray = 0;
	m_datareader = 0;
}
CSubscriber::~CSubscriber()
{
	if(m_datareader)
		delete m_datareader;
}

void CSubscriber::Assemble(CDataFlow* pDataFlow , CReceivingArray* pReceivingArray)
{
	m_dataflow = pDataFlow;
	m_receivingarray = pReceivingArray;
}
int CSubscriber::Subscribe(TopicID topic)
{
	if(m_dataflow)
	{
		return m_dataflow->Subscribe(topic);
	}
	else
	{
		return -1;
	}
	
}
int CSubscriber::UnSubscribe(TopicID topic)
{
	if(m_dataflow)
	{
		return m_dataflow->UnSubscribe(topic);
	}
	else
	{
		return -1;
	}
}
void CSubscriber::Run()
{
}
CDataReader* CSubscriber::GetReader()
{
	if(m_datareader)
		return m_datareader;
	else
	{
		m_datareader = new CDataReader;
		m_datareader->SetSubscriber(this);
		return m_datareader;
	}
}

TopicID CSubscriber::Read(CData& data)
{
	TopicID tmptopic = -1;
	char* tmpdatabuf = 0;
	int tmplength = 0;
	m_receivingarray->Receive(tmptopic , tmpdatabuf , tmplength); 
	//这里要求m_receivingarray提供新的内存拷贝,然后将原有拷贝删除
	CData tmpdata;
	if(tmptopic == SUBSCRIBE_MONITOR_INFO)
	{
		tmpdata.DecodeCompState(tmpdatabuf);
	}
	else
	{
		tmpdata.Decode(tmpdatabuf);
	}	
	free(tmpdatabuf);
	data = tmpdata;
	return tmptopic;
}