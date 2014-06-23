#ifndef CDATAFLOW_H
#define CDATAFLOW_H

class CDataFlow:public CThread,public CSender{

public:
	
	CDataFlow();

	~CDataFlow();
	
	int Assemble(CDataChannel* pDataChannel);

	int Assemble(CDataArray* pSendingArray , CDataArray* pReceivingArray);

	/******************************************************************
	 完成自身主题的发布，在CFlowOut中建立相应结构
	******************************************************************/
	int Publish(TopicID topic);
	
	/******************************************************************
	 完成自身所需主题的订阅
	******************************************************************/
	int Subscribe(TopicID topic);
	
	/******************************************************************
	 解除自身主题的发布,删除CFlowOut中建立相应结构以及CSendingArray中的队列
	******************************************************************/
	int UnPublish(TopicID topic);
	
	/******************************************************************
	 解除自身主题的订阅,删除CDataFlow以及CReceivingArray中的结构
	******************************************************************/
	int UnSubscribe(TopicID topic);

	/******************************************************************
	 CDataChannel接收到来自TCP连接的订阅请求之后，调用该接口
	******************************************************************/
	int RemoteSubscribe(TopicID topic);
	
	/*CDataChannel接收到组播信息后调用*/
	int RemotePublish(TopicID topic , char* pAddr , unsigned short port);
	
	/*CDataChannel调用，解除发布报文（自身所需要的订阅没人发布了），建立预先订阅结构*/
	int RemoteUnpublish(TopicID topic);

	/*CDataChannel调用，解除订阅报文,删除主题对应的队列*/
	int RemoteUnsubscribe(TopicID topic);

	/******************************************************************
	由CDataChannel调用，完成底层所有接受数据（TCP）向上层的传递
	******************************************************************/
	int Send(TopicID topic,char* buffer , int len);
	
	/*****************************************************************
	1、	从CSendingArray中获取需要发送的数据提交给CDataChannel；
	2、	周期性从CFlowOut中获取发布信息，利用CDataChannel发送到组播地址
	******************************************************************/
	void Run();

	int RemotePublishFlowInfo(char* rawData);

private:
	
	void MulticastAndRefresh();

	CFlowOut* cfo_;
	CFlowIn* cfi_;
	CDataChannel* pDataChannel_;
	
	CDataArray* pSendingArray_;
	CDataArray* pReceivingArray_;
	
	struct TopicIP** tip_;//网络上发布的所有Topic对应的IPPort结构
	int tipSize; //tip_结构的实际长度
	//预先订阅结构
	int* preserve;

	bool subMonitorInfo;

	MUTEX lock;

};

#endif