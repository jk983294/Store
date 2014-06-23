#pragma once

class DLL_API CSubscriber : public CThread
{
public:
	CSubscriber();
	~CSubscriber();
	void Assemble(CDataFlow* pDataFlow , CReceivingArray* pReceivingArray);
	int Subscribe(TopicID topic);
	int UnSubscribe(TopicID topic);
	virtual void Run();
	CDataReader* GetReader();
	TopicID Read(CData& data);
private:
	CDataFlow* m_dataflow;
	CReceivingArray* m_receivingarray;
	CDataReader* m_datareader;

};