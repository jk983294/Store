#pragma once

const int ANY_TYPE_INT = 1;
const int ANY_TYPE_SHORT = 2;
const int ANY_TYPE_USHORT = 3;
const int ANY_TYPE_LONG = 4;
const int ANY_TYPE_ULONG = 5;
const int ANY_TYPE_FLOAT = 6;
const int ANY_TYPE_DOUBLE = 7;
const int ANY_TYPE_STRING = 8;
const int ANY_TYPE_BLOCK = 9;

const int ANY_TYPE_CHAR = 10;
const int ANY_TYPE_UCHAR = 11;
const int ANY_TYPE_SIGNEDCHAR = 12;
const int ANY_TYPE_LONGLONG = 13;
const int ANY_TYPE_ULONGLONG = 14;

const int ANY_TYPE_SIGNEDINT = 15;
const int ANY_TYPE_UINT = 16;


class DLL_API CAny
{
public:
	CAny(void);
	CAny(CAny& any);
	~CAny(void);

	int Type();
	int Length();
	void* Value();
	int Encode(void*& buf);
	int Decode(void* buf , int len);


	CAny & operator= (CAny &any);

	void operator<<= (int &val);
	void operator<<= (const int &val);
	void operator<<= (short &val);
	void operator<<= (const short &val);
	void operator<<= (unsigned short &val);
	void operator<<= (const unsigned short &val);
	void operator<<= (long &val);
	void operator<<= (const long &val);
	void operator<<= (unsigned long &val);
	void operator<<= (const unsigned long &val);
	void operator<<= (float &val);
	void operator<<= (const float &val);
	void operator<<= (double &val);
	void operator<<= (const double &val);
	void operator<<= (char *val);
	void operator<<= (const char *val);
	void operator<<= (void *val);
	void InsertBuf(void* val , size_t len);

	void operator<<= (char &val);
	void operator<<= (const char &val);
	void operator<<= (unsigned char &val);
	void operator<<= (signed char &val);
	void operator<<= (LONGLONG &val);
	void operator<<= (const LONGLONG val);
	void operator<<= (ULONGLONG &val);
	void operator<<= (const ULONGLONG val);
	void operator<<= (signed __int32 val);
	void operator<<= (unsigned __int32 val);

	

	void operator>>= (int &val);
	void operator>>= (short &val);
	void operator>>= (unsigned short &val);
	void operator>>= (long &val);
	void operator>>= (unsigned long &val);
	void operator>>= (float &val);
	void operator>>= (double &val);
	void operator>>= (char *&val);
	void operator>>= (void *&val);

	void operator>>= (char &val);
	void operator>>= (unsigned char &val);
	void operator>>= (signed char &val);
	void operator>>= (LONGLONG &val);
	void operator>>= (ULONGLONG &val);
	void operator>>= (signed __int32 &val);
	void operator>>= (unsigned __int32 &val);
	void* GetBuf(size_t& len);

private:
	void* m_data;
	int m_type;
	int m_length;
};
