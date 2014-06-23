#include "../include/DataBus.h"

CData::CData(void)
{
	m_length = 0;
	for(u_short i=0;i<MAX_CDATA_LENGTH;i++)
	{
		m_data[i] = 0;
	}
}

CData::CData(CData& data)
{
	m_length = 0;
	for(u_short i=0;i<MAX_CDATA_LENGTH;i++)
	{
		m_data[i] = 0;
	}
	void* buf = 0;
	data.Encode(buf);
	Decode(buf);
	free(buf);
}

CData::~CData(void)
{
	for(u_short i=0;i<MAX_CDATA_LENGTH;i++)
	{
		if(m_data[i] != 0)
		{
			delete m_data[i];
			m_data[i] = 0;
		}
	}
}

CData& CData::operator=(CData data)
{
	for(u_short i=0;i<MAX_CDATA_LENGTH;i++)
	{
		if(m_data[i] != 0)
		{
			delete m_data[i];
			m_data[i] = 0;
		}
	}
	void* buf = 0;
	data.Encode(buf);
	Decode(buf);
	free(buf);
	return *this;
}

void CData::SetTopicID(TopicID topic)
{
	m_topic = topic;
}
TopicID CData::GetTopic()
{
	return m_topic;
}

int CData::Length(int length)
{
	for(u_short i=0;i<MAX_CDATA_LENGTH;i++)
	{
		if(m_data[i] != 0)
		{
			delete m_data[i];
			m_data[i] = 0;
		}
	}
	if(length<=MAX_CDATA_LENGTH && length>0)
	{
		m_length = length;
		for(u_short i=0;i<m_length;i++)
		{
			m_data[i] = new CAny;
		}
		return m_length;
	}
	else
	{
		return -1;
	}
}

int CData::Length()
{
	return m_length;
}

CAny& CData::operator [](int index)
{
	return *(m_data[index]);
}

int CData::Encode(void*& buf)
{
	int tmplength = 2*sizeof(int);
	int tmpanylength[MAX_CDATA_LENGTH];
	void* tmpanybuf[MAX_CDATA_LENGTH];
	char* tmpbuf = 0;
	u_short index = 0;
	for(index=0;index<m_length;index++)
	{
		if(m_data[index]!=0)
		{
			tmpanylength[index] = m_data[index]->Encode(tmpanybuf[index]);
			tmplength+=(tmpanylength[index]+sizeof(int));
		}
		else
			break;
	}
	buf = malloc(tmplength);
	tmpbuf = (char*)buf;
	memcpy(tmpbuf , (void*)(&m_topic) , sizeof(int));
	tmpbuf+=sizeof(int);
	memcpy(tmpbuf , (void*)(&m_length) , sizeof(int));
	tmpbuf+=sizeof(int);
	for(index=0;index<m_length;index++)
	{
		if(m_data[index]!=0)
		{
			memcpy(tmpbuf , (void*)(&(tmpanylength[index])) , sizeof(int));
			tmpbuf+=sizeof(int);
			memcpy(tmpbuf , tmpanybuf[index] , tmpanylength[index]);
			tmpbuf += tmpanylength[index];
			free(tmpanybuf[index]);
		}
	}
	return tmplength;

}
int CData::Decode(void* buf)
{
	u_short index = 0;
	for(index=0;index<MAX_CDATA_LENGTH;index++)
	{
		if(m_data[index] != 0)
		{
			delete m_data[index];
			m_data[index] = 0;
		}
	}
	char* tmpbuf = (char*)buf;
	m_topic = *((int*)tmpbuf);
	tmpbuf+=sizeof(int);
	m_length = *((int*)tmpbuf);
	tmpbuf+=sizeof(int);
	for(index=0;index<m_length;index++)
	{
		int tmpanylength = *((int*)tmpbuf);
		tmpbuf+=sizeof(int);
		m_data[index] = new CAny;
		m_data[index]->Decode(tmpbuf , tmpanylength);
		tmpbuf+=tmpanylength;
	}
	return m_length;
}

int CData::DecodeCompState(void* buf)
{
	char* tmpbuf = (char*)buf;
	char tmpseg;
	int topic =0;

	topic = (((unsigned char)tmpbuf[1])<<24)|
						(((unsigned char)tmpbuf[2])<<16)|
						(((unsigned char)tmpbuf[3])<<8)|
						(((unsigned char)tmpbuf[4]));


	char tmpip[32]={0};
	char tempchar[6]={0};
	for (int i=0;i<4;i++)
	{
		sprintf(tempchar,"%d",(unsigned char)tmpbuf[8-i]);
		strcat(tmpip,tempchar);
		strcat(tmpip,".");
	}

	u_short tmpport = (((unsigned char)tmpbuf[9])<<8)|
		(((unsigned char)tmpbuf[10]));

	tmpseg = tmpbuf[11];
	int tmpdirection = (int)tmpseg;
	u_short tmpspeed = (((unsigned char)tmpbuf[12])<<8)|
		(((unsigned char)tmpbuf[13]));

	m_topic = 1;
	m_length = 5;
	m_data[0] = new CAny;
	(*(m_data[0]))<<=topic;
	m_data[1] = new CAny;
	(*(m_data[1]))<<=tmpip;
	m_data[2] = new CAny;
	(*(m_data[2]))<<=tmpport;
	m_data[3] = new CAny;
	(*(m_data[3]))<<=tmpdirection;
	m_data[4] = new CAny;
	(*(m_data[4]))<<=tmpspeed;
	return 1;

}