#pragma once

class DLL_API CPublisher
{
public:
	CPublisher(void);
	~CPublisher(void);

	void Assemble(CDataFlow* pDataFlow , CSendingArray* pSendingArray);
	int Publish(TopicID topic);
	int UnPublish(TopicID topic);
	int Send(CData data);
	CDataWriter* GetWriter();

private:
	CDataFlow* m_dataflow;
	CSendingArray* m_sendingarray;
	CDataWriter* m_datawriter;

};
