#include "../include/DataBus.h"

CAny::CAny(void)
{
	m_data = 0;
}

CAny::CAny(CAny& any)
{
	m_type = any.Type();
	m_length = any.Length();
	m_data = malloc(m_length);
	if(m_type == ANY_TYPE_STRING)
	{
		strcpy((char*)m_data , (char*)(any.Value()));
	}
	else
	{
		memcpy(m_data , any.Value() , m_length);
	}
}

CAny::~CAny(void)
{
	if(m_data)
	{
		free(m_data);
		m_data = 0;
	}
}

int CAny::Type()
{
	return m_type;
}
int CAny::Length()
{
	return m_length;
}
void* CAny::Value()
{
	return m_data;
}

int CAny::Encode(void*& buf)
{
	int tmplength = sizeof(int) + m_length;
	buf = malloc(tmplength);
	memcpy(buf , (void*)(&m_type) , sizeof(int));
	memcpy((char*)buf+sizeof(int) , m_data , m_length);
	return tmplength;
}

int CAny::Decode(void* buf , int len)
{
	if(m_data)
	{
		free(m_data);
		m_data = 0;
	}
	m_type = *(int*)buf;
	m_data = malloc(len - sizeof(int));
	m_length = len - sizeof(int);
	memcpy(m_data , (char*)buf+sizeof(int) , len - sizeof(int));
	return m_type;
}

CAny & CAny::operator= (CAny &any)
{
	if(m_data)
	{
		free(m_data);
		m_data = 0;
	}
	m_type = any.Type();
	m_length = any.Length();
	m_data = malloc(m_length);
	if(m_type == ANY_TYPE_STRING)
	{
		strcpy((char*)m_data , (char*)(any.Value()));
	}
	else
	{
		memcpy(m_data , any.Value() , m_length);
	}
	return *this;
}

void CAny::operator<<= (int &val)
{
	if(m_data)
	{
		free(m_data);
		m_data = 0;
	}
	m_type = ANY_TYPE_INT;
	m_length = sizeof(int);
	m_data = malloc(m_length);
	memcpy(m_data , (void*)&val , m_length);
	return;
}
void CAny::operator<<= (const int &val)
{
	if(m_data)
	{
		free(m_data);
		m_data = 0;
	}
	m_type = ANY_TYPE_INT;
	m_length = sizeof(int);
	m_data = malloc(m_length);
	memcpy(m_data , (void*)&val , m_length);
	return;
}

void CAny::operator<<= (short &val)
{
	if(m_data)
	{
		free(m_data);
		m_data = 0;
	}
	m_type = ANY_TYPE_SHORT;
	m_length = sizeof(short);
	m_data = malloc(m_length);
	memcpy(m_data , (void*)&val , m_length);
	return;
}
void CAny::operator<<= (const short &val)
{
	if(m_data)
	{
		free(m_data);
	}
	m_type = ANY_TYPE_SHORT;
	m_length = sizeof(short);
	m_data = malloc(m_length);
	*(int*)m_data = val;
	return;
}

void CAny::operator<<= (unsigned short &val)
{
	if(m_data)
	{
		free(m_data);
		m_data = 0;
	}
	m_type = ANY_TYPE_USHORT;
	m_length = sizeof(unsigned short);
	m_data = malloc(m_length);
	memcpy(m_data , (void*)&val , m_length);
	return;
}
void CAny::operator<<= (const unsigned short &val)
{
	if(m_data)
	{
		free(m_data);
	}
	m_type = ANY_TYPE_USHORT;
	m_length = sizeof(unsigned short);
	m_data = malloc(m_length);
	*(unsigned short*)m_data = val;
	return;
}
void CAny::operator<<= (long &val)
{
	if(m_data)
	{
		free(m_data);
		m_data = 0;
	}
	m_type = ANY_TYPE_LONG;
	m_length = sizeof(long);
	m_data = malloc(m_length);
	memcpy(m_data , (void*)&val , m_length);
	return;
}
void CAny::operator<<= (const long &val)
{
	if(m_data)
	{
		free(m_data);
	}
	m_type = ANY_TYPE_LONG;
	m_length = sizeof(long);
	m_data = malloc(m_length);
	*(long*)m_data = val;
	return;
}
void CAny::operator<<= (unsigned long &val)
{
	if(m_data)
	{
		free(m_data);
		m_data = 0;
	}
	m_type = ANY_TYPE_ULONG;
	m_length = sizeof(unsigned long);
	m_data = malloc(m_length);
	memcpy(m_data , (void*)&val , m_length);
	return;
}
void CAny::operator<<= (const unsigned long &val)
{
	if(m_data)
	{
		free(m_data);
	}
	m_type = ANY_TYPE_ULONG;
	m_length = sizeof(unsigned long);
	m_data = malloc(m_length);
	*(long*)m_data = val;
	return;
}
void CAny::operator<<= (float &val)
{
	if(m_data)
	{
		free(m_data);
		m_data = 0;
	}
	m_type = ANY_TYPE_FLOAT;
	m_length = sizeof(float);
	m_data = malloc(m_length);
	memcpy(m_data , (void*)&val , m_length);
	return;
}
void CAny::operator<<= (const float &val)
{
	if(m_data)
	{
		free(m_data);
	}
	m_type = ANY_TYPE_FLOAT;
	m_length = sizeof(float);
	m_data = malloc(m_length);
	*(float*)m_data = val;
	return;
}
void CAny::operator<<= (double &val)
{
	if(m_data)
	{
		free(m_data);
		m_data = 0;
	}
	m_type = ANY_TYPE_DOUBLE;
	m_length = sizeof(double);
	m_data = malloc(m_length);
	memcpy(m_data , (void*)&val , m_length);
	return;
}
void CAny::operator<<= (const double &val)
{
	if(m_data)
	{
		free(m_data);
	}
	m_type = ANY_TYPE_DOUBLE;
	m_length = sizeof(double);
	m_data = malloc(m_length);
	*(double*)m_data = val;
	return;
}
void CAny::operator<<= (char *val)
{
	if(m_data)
	{
		free(m_data);
		m_data = 0;
	}
	m_type = ANY_TYPE_STRING;
	m_length = strlen(val)+1;
	m_data = malloc(m_length);
	strcpy((char*)m_data , val);
	return;
}
void CAny::operator<<= (const char *val)
{
	if(m_data)
	{
		free(m_data);
		m_data = 0;
	}
	m_type = ANY_TYPE_STRING;
	m_length = strlen(val)+1;
	m_data = malloc(m_length);
	strcpy((char*)m_data , val);
	return;
}

void CAny::operator<<= (void *val)
{
	if(m_data)
	{
		free(m_data);
		m_data = 0;
	}
	m_type = ANY_TYPE_BLOCK;
	m_length = sizeof(val);
	m_data = malloc(m_length);
	memcpy(m_data , val , m_length);
	return;
}

void CAny::operator<<= (char &val)
{
	if(m_data)
	{
		free(m_data);
		m_data = 0;
	}
	m_type = ANY_TYPE_CHAR;
	m_length = sizeof(char);
	m_data = malloc(m_length);
	memcpy(m_data , (void*)&val , m_length);
	return;
}

void CAny::operator<<= (const char &val)
{
	if(m_data)
	{
		free(m_data);
		m_data = 0;
	}
	m_type = ANY_TYPE_CHAR;
	m_length = sizeof(char);
	m_data = malloc(m_length);
	memcpy(m_data , (void*)&val , m_length);
	return;
}

void CAny::operator<<= (unsigned char &val)
{
	if(m_data)
	{
		free(m_data);
		m_data = 0;
	}
	m_type = ANY_TYPE_UCHAR;
	m_length = sizeof(unsigned char);
	m_data = malloc(m_length);
	memcpy(m_data , (void*)&val , m_length);
	return;
}

void CAny::operator<<= (signed char &val)
{
	if(m_data)
	{
		free(m_data);
		m_data = 0;
	}
	m_type = ANY_TYPE_SIGNEDCHAR;
	m_length = sizeof(signed char);
	m_data = malloc(m_length);
	memcpy(m_data , (void*)&val , m_length);
	return;
}

void CAny::InsertBuf(void* val , size_t len)
{
	if(m_data)
	{
		free(m_data);
		m_data = 0;
	}
	m_type = ANY_TYPE_BLOCK;
	m_length = len;
	m_data = malloc(m_length);
	memcpy(m_data , val , m_length);
	return;
}

void CAny::operator<<= (LONGLONG &val)
{
	if(m_data)
	{
		free(m_data);
		m_data = 0;
	}
	m_type = ANY_TYPE_LONGLONG;
	m_length = sizeof(LONGLONG);
	m_data = malloc(m_length);
	memcpy(m_data , (void*)&val , m_length);
	return;
}
void CAny::operator<<= (const LONGLONG val)
{
	if(m_data)
	{
		free(m_data);
	}
	m_type = ANY_TYPE_LONGLONG;
	m_length = sizeof(LONGLONG);
	m_data = malloc(m_length);
	*(LONGLONG*)m_data = val;
	return;
}
void CAny::operator<<= (ULONGLONG &val)
{
	if(m_data)
	{
		free(m_data);
		m_data = 0;
	}
	m_type = ANY_TYPE_ULONGLONG;
	m_length = sizeof(ULONGLONG);
	m_data = malloc(m_length);
	memcpy(m_data , (void*)&val , m_length);
	return;
}
void CAny::operator<<= (const ULONGLONG val)
{
	if(m_data)
	{
		free(m_data);
	}
	m_type = ANY_TYPE_ULONGLONG;
	m_length = sizeof(ULONGLONG);
	m_data = malloc(m_length);
	*(ULONGLONG*)m_data = val;
	return;
}

void CAny::operator<<= (signed __int32 val)
{
	if(m_data)
	{
		free(m_data);
		m_data = 0;
	}
	m_type = ANY_TYPE_SIGNEDINT;
	m_length = sizeof(signed __int32);
	m_data = malloc(m_length);
	memcpy(m_data , (void*)&val , m_length);
	return;
}

void CAny::operator<<= (unsigned __int32 val)
{
	if(m_data)
	{
		free(m_data);
		m_data = 0;
	}
	m_type = ANY_TYPE_UINT;
	m_length = sizeof(unsigned __int32);
	m_data = malloc(m_length);
	memcpy(m_data , (void*)&val , m_length);
	return;
}

void CAny::operator>>= (int &val)
{
	if(m_type == ANY_TYPE_INT)
	{
		val = *(int*)m_data;
	}
	else
	{
		val = 0;
		return;
	}
}

void CAny::operator>>= (short &val)
{
	if(m_type == ANY_TYPE_SHORT)
	{
		val = *(short*)m_data;
	}
	else
	{
		val = 0;
		return;
	}
}

void CAny::operator>>= (unsigned short &val)
{
	if(m_type == ANY_TYPE_USHORT)
	{
		val = *(unsigned short*)m_data;
	}
	else
	{
		val = 0;
		return;
	}
}
void CAny::operator>>= (long &val)
{
	if(m_type == ANY_TYPE_LONG)
	{
		val = *(long*)m_data;
	}
	else
	{
		val = 0;
		return;
	}
}
void CAny::operator>>= (unsigned long &val)
{
	if(m_type == ANY_TYPE_ULONG)
	{
		val = *(unsigned long*)m_data;
	}
	else
	{
		val = 0;
		return;
	}
}
void CAny::operator>>= (float &val)
{
	if(m_type == ANY_TYPE_FLOAT)
	{
		val = *(float*)m_data;
	}
	else
	{
		val = 0;
		return;
	}
}
void CAny::operator>>= (double &val)
{
	if(m_type == ANY_TYPE_DOUBLE)
	{
		val = *(double*)m_data;
	}
	else
	{
		val = 0;
		return;
	}
}
void CAny::operator>>= (char *&val)
{
	if(m_type == ANY_TYPE_STRING)
	{
		val = _strdup((char*)m_data);
	}
	else
	{
		val = 0;
		return;
	}
}

void CAny::operator>>= (void *&val)
{
	if(m_type == ANY_TYPE_BLOCK)
	{
		val = malloc(m_length);
		memcpy(val , m_data , m_length);
	}
	else
	{
		val = 0;
		return;
	}
}

void* CAny::GetBuf(size_t& len)
{
	void* val = 0;
	if(m_type == ANY_TYPE_BLOCK)
	{
		val = malloc(m_length);
		memcpy(val , m_data , m_length);
		len = m_length;
		return val;
	}
	else
	{
		val = 0;
		len = 0;
		return val;
	}	
}

void CAny::operator>>= (char &val)
{
	if(m_type == ANY_TYPE_CHAR)
	{
		val = *(char*)m_data;
	}
	else
	{
		val = 0;
		return;
	}
}
void CAny::operator>>= (unsigned char &val)
{
	if(m_type == ANY_TYPE_UCHAR)
	{
		val = *(unsigned char*)m_data;
	}
	else
	{
		val = 0;
		return;
	}
}
void CAny::operator>>= (signed char &val)
{
	if(m_type == ANY_TYPE_SIGNEDCHAR)
	{
		val = *(signed char*)m_data;
	}
	else
	{
		val = 0;
		return;
	}
}

void CAny::operator>>= (LONGLONG &val)
{
	if(m_type == ANY_TYPE_LONGLONG)
	{
		val = *(LONGLONG*)m_data;
	}
	else
	{
		val = 0;
		return;
	}
}
void CAny::operator>>= (ULONGLONG &val)
{
	if(m_type == ANY_TYPE_ULONGLONG)
	{
		val = *(ULONGLONG*)m_data;
	}
	else
	{
		val = 0;
		return;
	}
}

void CAny::operator>>= (signed __int32 &val)
{
	if(m_type == ANY_TYPE_SIGNEDINT)
	{
		val = *(signed __int32*)m_data;
	}
	else
	{
		val = 0;
		return;
	}
}

void CAny::operator>>= (unsigned __int32 &val)
{
	if(m_type == ANY_TYPE_UINT)
	{
		val = *(unsigned __int32*)m_data;
	}
	else
	{
		val = 0;
		return;
	}
}