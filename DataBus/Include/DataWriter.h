#pragma once

class DLL_API CDataWriter
{
public:
	CDataWriter(void);
	~CDataWriter(void);

	void SetPublisher(CPublisher* publisher);
	int Send(CData* data);
private:
	CPublisher* m_publisher;
};
