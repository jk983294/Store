
#pragma once

class DLL_API DomainParticipant  
{
public:
	DomainParticipant();
	virtual ~DomainParticipant();

	void SetDomainID(domain_id domain);

	CPublisher* create_publisher();
	void delete_publisher(CPublisher* publisher); 
	CSubscriber* create_subscriber();
	void delete_subscriber(CSubscriber* subscriber);
	CTopic* create_topic();
	void delete_topic(CTopic* topic);
	CData* create_data();
	void delete_data(CData* data);
private:
	domain_id m_domain;
	CPublisher* m_publisher;
	CSubscriber* m_subscriber;
};
