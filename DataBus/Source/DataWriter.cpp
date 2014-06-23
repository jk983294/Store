#include "../include/DataBus.h"

CDataWriter::CDataWriter(void)
{
	m_publisher = 0;
}

CDataWriter::~CDataWriter(void)
{
}

void CDataWriter::SetPublisher(CPublisher* publisher)
{
	m_publisher = publisher;
}
int CDataWriter::Send(CData* data)
{
	if(m_publisher)
		return m_publisher->Send(*data);
	else
		return -1;
}
