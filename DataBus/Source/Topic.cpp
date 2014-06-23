#include "../include/DataBus.h"

DLL_API CTopic::CTopic(void)
{
	for(u_short i=0;i<MAX_TOPIC_SEGMENTS;i++)
	{
		m_segments[i] = 0;
	}
	m_segnum = 0;
	m_domain = -1;
}

DLL_API CTopic::~CTopic(void)
{
	for(u_short i=0;i<MAX_TOPIC_SEGMENTS;i++)
	{
		if(m_segments[i] != 0)
		{
			free(m_segments[i]);
		}
		else
		{
			break;
		}
	}
}

DLL_API CTopic::CTopic(CTopic& topic)
{
	m_domain = topic.GetDomain();
	m_segnum = topic.GetSegNum();
	for(u_short i=0;i<MAX_TOPIC_SEGMENTS;i++)
	{
		if(i<m_segnum)
			m_segments[i] = _strdup(topic.GetSegment(i));
		else
			m_segments[i] = 0;
	}
}

DLL_API CTopic& CTopic::operator=(CTopic& topic)
{
	m_domain = topic.GetDomain();
	m_segnum = topic.GetSegNum();
	for(u_short i=0;i<m_segnum;i++)
	{
		m_segments[i] = _strdup(topic.GetSegment(i));
	}
	return *this;
}

DLL_API void CTopic::SetDomain(domain_id domain)
{
	m_domain = domain;
}

DLL_API int CTopic::AddSegment(char* pSegment)
{
	if(pSegment == 0)
	{
		return -1;
	}
	else if(strlen(pSegment)>MAX_TOPIC_SEGLENG)
	{
		return -1;
	}
	else
	{
		m_segments[m_segnum++] = _strdup(pSegment);
	}
	return 1;
}

DLL_API int CTopic::AddSegment(const char* pSegment)
{
	if(pSegment == 0)
	{
		return -1;
	}
	else if(strlen(pSegment)>MAX_TOPIC_SEGLENG)
	{
		return -1;
	}
	else
	{
		m_segments[m_segnum++] = _strdup(pSegment);
	}
	return 1;
}

DLL_API int CTopic::GetID()
{
	if(m_segnum == 0)
		return -1;
	else
	{
		int tmpid = 0;
		for(u_short i=0;i<m_segnum;i++)
		{
			tmpid+=100*i;
			for(u_short j=0;j<strlen(m_segments[i]);j++)
			{
				tmpid+=(int)m_segments[i][j];
			}
		}
		return m_domain*DOMAIN_INTERVAL+tmpid;
	}
}

DLL_API int CTopic::GetDomain()
{
	return m_domain;
}

DLL_API int CTopic::GetSegNum()
{
	return m_segnum;
}

DLL_API char* CTopic::GetSegment(int index)
{
	return m_segments[index];
}
