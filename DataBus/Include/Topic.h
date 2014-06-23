#pragma once

class DLL_API CTopic
{
public:
	CTopic(void);
	~CTopic(void);
	CTopic(CTopic& topic);

	CTopic& operator=(CTopic& topic);

	void SetDomain(domain_id domain);
	int AddSegment(char* pSegment);
	int AddSegment(const char* pSegment);
	
	int GetID();
	int GetDomain();
	int GetSegNum();
	char* GetSegment(int index);
private:
	char* m_segments[MAX_TOPIC_SEGMENTS];
	int m_segnum;
	domain_id m_domain;
};
