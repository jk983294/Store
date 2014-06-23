#include "../include/DataBus.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DomainParticipantFactory::DomainParticipantFactory()
{

}

DomainParticipantFactory::~DomainParticipantFactory()
{

}

DomainParticipant* DomainParticipantFactory::create_participant(domain_id id)
{
	DomainParticipant* tmpparticipant = new DomainParticipant;
	tmpparticipant->SetDomainID(id);
	return tmpparticipant;
}

void DomainParticipantFactory::delete_participant(DomainParticipant* participant)
{
	delete participant;
}