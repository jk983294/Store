
#pragma once

class DLL_API DomainParticipantFactory  
{
public:
	DomainParticipantFactory();
	virtual ~DomainParticipantFactory();

	DomainParticipant* create_participant(domain_id id);

	void delete_participant(DomainParticipant* participant);
};
