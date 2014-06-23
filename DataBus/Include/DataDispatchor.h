#pragma once

#define OPIMP_SUBID_MAPPING(handler , op , topic) \
	if (g_datadispatcher_ptr_) \
	g_datadispatcher_ptr_->RegisterHandlerOperation(handler , op , topic)

#define OPIMP_SUBID_UNMAPPING(handler , op , topic) \
	if (g_datadispatcher_ptr_) \
	g_datadispatcher_ptr_->UnRegisterHandlerOperation(handler , op , topic)

struct OpIdPair
{
	int op_;
	TopicID topic_;
};

struct HandlerNode
{
	CDataHandler* handler_;
	struct OpIdPair* ops_[MAXOPERATIONUM];
};

class DLL_API CDataDispatchor : public CThread
{
public:
	CDataDispatchor(void);
	~CDataDispatchor(void);

	void Assemble(CReceivingArray* pReceivingArray);

	virtual void Run();


	void RegisterHandlerOperation(CDataHandler* handler , int op , TopicID topic);

	void UnRegisterHandlerOperation(CDataHandler* handler , int op , TopicID topic);

private:
	struct HandlerNode* m_handlers[MAXINFOHANDLERNUM];
	CReceivingArray* m_receivingarray;
	MUTEX m_datadispatcherlock;


};
