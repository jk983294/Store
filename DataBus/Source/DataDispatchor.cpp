#include "../include/DataBus.h"

CDataDispatchor::CDataDispatchor(void)
{
	MUTEX_INIT(m_datadispatcherlock);
	m_receivingarray = 0;
	for(u_short i=0;i<MAXINFOHANDLERNUM;i++)
	{
		m_handlers[i] = 0;
	}
}

CDataDispatchor::~CDataDispatchor(void)
{
	MUTEX_DESTROY(m_datadispatcherlock);
	for(u_short i=0;i<MAXINFOHANDLERNUM;i++)
	{
		if(m_handlers[i] != 0)
			delete m_handlers[i];
	}
}

void CDataDispatchor::Assemble(CReceivingArray* pReceivingArray)
{
	m_receivingarray = pReceivingArray;
}

void CDataDispatchor::Run()
{
	while(1)
	{
		TopicID tmptopic = -1;
		char* tmpdatabuf = 0;
		int tmplength = 0;
		m_receivingarray->Receive(tmptopic , tmpdatabuf , tmplength); 
		//这里要求m_receivingarray提供新的内存拷贝,然后将原有拷贝删除
		CData tmpdata;
		if(tmptopic == SUBSCRIBE_MONITOR_INFO)
		{
			tmpdata.DecodeCompState(tmpdatabuf);
		}
		else
		{
			tmpdata.Decode(tmpdatabuf);
		}	
		free(tmpdatabuf);
		MUTEX_LOCK(m_datadispatcherlock);
		for(u_short i=0;i<MAXINFOHANDLERNUM;i++)
		{
			if(m_handlers[i] != 0)
			{
				for(u_short j=0;j<MAXOPERATIONUM;j++)
				{
					if(m_handlers[i]->ops_[j]!=0)
					{
						if(m_handlers[i]->ops_[j]->topic_ == tmptopic) //找到了相应的id
						{
							m_handlers[i]->handler_->OpDispatch(m_handlers[i]->ops_[j]->op_ , tmpdata);
						}
					}
				}
			}
			else
				break;
		}
		MUTEX_UNLOCK(m_datadispatcherlock);
	}
}

void CDataDispatchor::RegisterHandlerOperation(CDataHandler* handler , int op , TopicID topic)
{
	u_short index = 0;
	MUTEX_LOCK(m_datadispatcherlock);
	for(index=0;index<MAXINFOHANDLERNUM;index++)
	{
		if(m_handlers[index] != 0)
		{
			if(m_handlers[index]->handler_ == handler)
			{
				for(u_short j=0;j<MAXOPERATIONUM;j++)
				{
					if(m_handlers[index]->ops_[j]==0)
					{
						struct OpIdPair* tmpnode = new struct OpIdPair;
						tmpnode->topic_ = topic;
						tmpnode->op_ = op;
						m_handlers[index]->ops_[j] = tmpnode;
						MUTEX_UNLOCK(m_datadispatcherlock);
						return;
					}
				}
			}

		}
	}
	for(index=0;index<MAXINFOHANDLERNUM;index++)
	{
		if(m_handlers[index] == 0)
		{
			struct HandlerNode* tmpnode = new struct HandlerNode;
			for(u_short j=0;j<MAXOPERATIONUM;j++)
			{
				tmpnode->ops_[j] = 0;
			}
			struct OpIdPair* tmpidnode = new struct OpIdPair;
			tmpidnode->topic_ = topic;
			tmpidnode->op_ = op;
			tmpnode->ops_[0] = tmpidnode;
			tmpnode->handler_ = handler;
			m_handlers[index] = tmpnode;
			MUTEX_UNLOCK(m_datadispatcherlock);
			return;
		}
	}
	MUTEX_UNLOCK(m_datadispatcherlock);
	return;

}

void CDataDispatchor::UnRegisterHandlerOperation(CDataHandler* handler , int op , TopicID topic)
{
	int index = 0;
	MUTEX_LOCK(m_datadispatcherlock);
	for(index=0;index<MAXINFOHANDLERNUM;index++)
	{
		if(m_handlers[index] != 0)
		{
			if(m_handlers[index]->handler_ == handler)
			{
				for(u_short j=0;j<MAXOPERATIONUM;j++)
				{
					if(m_handlers[index]->ops_[j]!=0)
					{
						if(m_handlers[index]->ops_[j]->topic_ == topic &&
							m_handlers[index]->ops_[j]->op_ == op)
						{
							delete m_handlers[index]->ops_[j];
							m_handlers[index]->ops_[j] = 0;
							MUTEX_UNLOCK(m_datadispatcherlock);
							return;
						}
					}
				}
			}

		}
	}
	MUTEX_UNLOCK(m_datadispatcherlock);
	return;
}
