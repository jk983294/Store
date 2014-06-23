#include "../include/DataBus.h"

#pragma data_seg("shared")
//进程间共享的Port号的定义
u_short _globalPort=DEFAULT_PORT;
HANDLE mutex=CreateMutex(0, FALSE, 0);
#pragma data_seg()
#pragma comment(linker,"/SECTION:shared,RWS")

//进程内使用的PORT_ACCEPT
u_short PORT_ACCEPT=DEFAULT_PORT;

//网络初始化标志，如果进程已完成了网络初始化则为1
u_short _globalNetInitialized = 0;

void SetPortNumber(u_short port){
	PORT_ACCEPT=port;
}

u_short GetPortNumber()
{
	int ret;
	MUTEX_LOCK(mutex);
	_globalPort++;
	ret=_globalPort;
	MUTEX_UNLOCK(mutex);
	return ret;
}

u_short NetInitialize()
{

		PORT_ACCEPT=GetPortNumber();
		printf("组件分配端口号：%d\n",PORT_ACCEPT);
		printf("WSAStartup 初始化\n");
		WSADATA  Ws;
		if ( WSAStartup(MAKEWORD(2,2), &Ws) != 0 )
		{
			return 0;
		}
		else
		{
			return 1;
		}
	
		return 1;

}


DLL_API CDataDispatchor* g_datadispatcher_ptr_;
DLL_API CDataFlow* g_dataflow_ptr_;
DLL_API CSendingArray* g_sendingarray_ptr_;
DLL_API CReceivingArray* g_receivingarray_ptr_;
DLL_API CNetworkOperator* g_networkoperator_ptr_;
DLL_API DomainParticipantFactory* g_domainparticipantfactory_ptr_;
DLL_API DomainParticipant* g_domainparticipant_ptr_;
DLL_API CPublisher* g_publisher_ptr_;
DLL_API CSubscriber* g_subscriber_ptr_;

DLL_API int Initialize(domain_id domain)
{
	MUTEX_LOCK(mutex);
	if(!_globalNetInitialized)
	{
		if(!NetInitialize())
		{
			printf("网络初始化不成功\n");
			MUTEX_UNLOCK(mutex);
			return -1;
		}
		g_dataflow_ptr_ = new CDataFlow();
		g_sendingarray_ptr_ = new CSendingArray();
		g_receivingarray_ptr_ = new CReceivingArray();
		g_networkoperator_ptr_ = new CNetworkOperator();
		if(g_networkoperator_ptr_->Initialize(domain)==-1)
		{
			_globalNetInitialized = 0;
			printf("NetworkOperator initialize failure.\n");
			MUTEX_UNLOCK(mutex);
			return _globalNetInitialized; 
		}
		g_networkoperator_ptr_->Assemble(g_dataflow_ptr_ , g_sendingarray_ptr_ , g_receivingarray_ptr_);

		g_datadispatcher_ptr_ = new CDataDispatchor();
		g_domainparticipantfactory_ptr_ = new DomainParticipantFactory();
		g_domainparticipant_ptr_ = g_domainparticipantfactory_ptr_->create_participant(domain);
		g_publisher_ptr_ = g_domainparticipant_ptr_->create_publisher();
		g_subscriber_ptr_ = g_domainparticipant_ptr_->create_subscriber();

		g_publisher_ptr_->Assemble(g_dataflow_ptr_ , g_sendingarray_ptr_);
		g_subscriber_ptr_->Assemble(g_dataflow_ptr_ , g_receivingarray_ptr_);
		g_datadispatcher_ptr_->Assemble(g_receivingarray_ptr_);

		g_datadispatcher_ptr_->Start();

		g_dataflow_ptr_->Start();

		_globalNetInitialized = 1;
	}
	MUTEX_UNLOCK(mutex);
	return _globalNetInitialized;
}

DLL_API CTopic CreateTopic()
{
	CTopic rst;
	CTopic* tmptopic = g_domainparticipant_ptr_->create_topic();
	rst = *tmptopic;
	delete tmptopic;
	return rst;
}
DLL_API CData CreateData(CTopic topic)
{
	CData* tmpdata = g_domainparticipant_ptr_->create_data();
	tmpdata->SetTopicID(topic.GetID());
	CData rst = *tmpdata;
	delete tmpdata;
	return rst;
}

DLL_API int Publish(CTopic topic)
{
	return g_publisher_ptr_->Publish(topic.GetID());
}
DLL_API int UnPublish(CTopic topic)
{
	return g_publisher_ptr_->UnPublish(topic.GetID());
}
DLL_API int UnPublish(TopicID topic)
{
	return -1;
}
DLL_API int Subscribe(CTopic topic)
{
	return g_subscriber_ptr_->Subscribe(topic.GetID());
}
DLL_API int Subscribe(TopicID topic)
{
	return g_subscriber_ptr_->Subscribe(topic);
}
DLL_API int UnSubscribe(CTopic topic)
{
	return g_subscriber_ptr_->UnSubscribe(topic.GetID());
}
DLL_API int UnSubscribe(TopicID topic)
{
	return -1;
}
DLL_API int Send(CData data)
{
	return g_publisher_ptr_->Send(data);
}
