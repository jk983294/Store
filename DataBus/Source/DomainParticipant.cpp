#include "../include/DataBus.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DomainParticipant::DomainParticipant()
{
	m_domain = -1;
	m_publisher = 0;
	m_subscriber = 0;
}

DomainParticipant::~DomainParticipant()
{
	if(m_publisher)
		delete m_publisher;
	if(m_subscriber)
		delete m_subscriber;

}

void DomainParticipant::SetDomainID(domain_id domain)
{
	m_domain = domain;
}

CPublisher* DomainParticipant::create_publisher()
{
	if(m_publisher)
	{
		return m_publisher;
	}
	else
	{
		m_publisher = new CPublisher;
		return m_publisher;
	}
}

void DomainParticipant::delete_publisher(CPublisher* publisher)
{
	return;
}

CSubscriber* DomainParticipant::create_subscriber()
{
	if(m_subscriber)
	{
		return m_subscriber;
	}
	else
	{
		m_subscriber = new CSubscriber;
		return m_subscriber;
	}
}
void DomainParticipant::delete_subscriber(CSubscriber* subscriber)
{
	return;
}
CTopic* DomainParticipant::create_topic()
{
	CTopic* tmptopic = new CTopic;
	tmptopic->SetDomain(m_domain);
	return tmptopic;
}
void DomainParticipant::delete_topic(CTopic* topic)
{
	delete topic;
}
CData* DomainParticipant::create_data()
{
	return new CData;
}
void DomainParticipant::delete_data(CData* data)
{
	delete data;
}
