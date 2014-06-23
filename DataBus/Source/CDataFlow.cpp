#include "../include/DataBus.h"

CDataFlow::CDataFlow(){
	cfo_=new CFlowOut();
	cfi_=new CFlowIn();
	//采用变长的数据结构，将来可以扩展
	MUTEX_INIT(lock);
	subMonitorInfo = false;
	tipSize=MAX_TOPIC_SIZE;
	tip_= new TopicIP*[tipSize];
	preserve=new int[tipSize];
	for(u_short i=0;i<tipSize;i++){
		tip_[i]=0;
		preserve[i]=-1;
	}
}

CDataFlow::~CDataFlow(){
	for(u_short i=0;i<tipSize;i++){
		if(tip_[i]!=0)
			delete tip_[i];
	}
	delete[] tip_;
	delete[] preserve;
	MUTEX_DESTROY(lock);
}

int CDataFlow::Assemble(CDataChannel* pDataChannel){
	pDataChannel_=pDataChannel;
	return 1;
}

int CDataFlow::Assemble(CDataArray* pSendingArray , CDataArray* pReceivingArray){
	pSendingArray_=pSendingArray;
	pReceivingArray_=pReceivingArray;
	return 1;
}

/*完成自身主题的发布，在CFlowOut中建立相应结构*/
int  CDataFlow::Publish(TopicID topic){
	cfo_->Publish(topic);
	//检查预先订阅结构
	for(u_short i=0;i<tipSize;i++)
	{
		if(preserve[i]==topic)
		{
			cfo_->LocalSubscribe(topic);
			pReceivingArray_->AddTopic(topic);
			pSendingArray_->AddTopic(topic);
			break;
		}
}
	return 1;
}

/*解除自身主题的发布,调用CDataChannel拆除链路，删除CFlowOut中建立相应结构以及CSendingArray中的队列*/
int CDataFlow::UnPublish(TopicID topic)
{
	cfo_->Unpublish(topic);
	pDataChannel_->UnPublish(topic);
	return pSendingArray_->RemoveTopic(topic);
}

/*CDataChannel接收到来自组播的主题发布信息之后调用该接口
1.维护tip结构
2.检查是否有预先定义结构
有则建立订阅（CFlowIn中建立结构并调用CDataChannel的Subscribe）
返回值：-1，tip结构满了;
		0,是自己发组播出去的topic，什么都不做
*/
int CDataFlow::RemotePublish(TopicID topic , char* pAddr , unsigned short port){
	//printf("收到组播数据:%d\n",topic);
	//判断是不是自己组播出去的
	if(cfo_->TopicExist(topic)){
		return 0;
	}
	u_short i=0;
	MUTEX_LOCK(lock);
	for(i=0;i<tipSize;i++){
		if(tip_[i]!=0 && tip_[i]->topic_==topic){
			//已经存在了该topic，更新
			strcpy(tip_[i]->ip,pAddr);
			tip_[i]->port=port;
			tip_[i]->reference++;//引用周期性+1
			break;
		}
	}
	
	if(i==tipSize)
	{
		//没找到,说明第一次接收到该topic的发布信息
		for (i=0;i<tipSize;i++)
		{
			if (tip_[i]==0)
			{
				tip_[i]=new struct TopicIP;
				tip_[i]->topic_=topic;
				strcpy(tip_[i]->ip,pAddr);
				tip_[i]->port=port;
				tip_[i]->reference =1;
				break;
			}
		}
	}
	MUTEX_UNLOCK(lock);
	
	if(i==tipSize){//满了……
		printf("Topic Over Sized in CDataFlow.RemotePublish!");
		return -1;
	}
	
	//最后别忘了检查是否有预先订阅结构
	for(i=0;i<tipSize;i++){
		if(preserve[i]==topic){
			//存在要订阅的主题，调用CDataChannel进行订阅
			pReceivingArray_->AddTopic(topic);
			pDataChannel_->Subscribe(topic,pAddr,port);
			cfi_->Subscribe(topic,pAddr,port);
			MUTEX_LOCK(lock);
			preserve[i]=-1;//订完了要注销预先订阅结构
			MUTEX_UNLOCK(lock);
			break;
		}
	}
	return 1;
}
/*CDataChannel接收到来自TCP连接的解除发布报文时，
调用CDataFlow的RemoteUnPublish接口。
CDataFlow删除自身保存的系统发布主题信息和CFlowIn中的已连接的系统发布主题信息
同时建立一个预先订阅结构。
进而调用CReceivingArray的RemoveTopic删除主题对应的队列*/
int CDataFlow::RemoteUnpublish(TopicID topic)
{
	int index = 0;
	MUTEX_LOCK(lock);
	for (index=0;index<tipSize;index++)	{
		if(tip_[index]!=0 && tip_[index]->topic_==topic){
			delete tip_[index];//别忘了释放内存
			tip_[index]=0;
			break;
		}
	}
	//预定义
	for(index=0;index<tipSize;index++){
		if(preserve[index]==-1){
			preserve[index]=topic;
			break;
		}
	}
	MUTEX_UNLOCK(lock);
	
	//删除已经建立了连接的系统发布主题信息
	cfi_->Unsubscribe(topic);
	
	return pReceivingArray_->RemoveTopic(topic);

}

/*完成自身所需主题的订阅，检查tip_中有没有该topic：
若有，建立订阅（CFlowIn中建立结构并调用CDataChannel的Subscribe），返回1。
若没有，建立预先订阅机构，返回0。
极端情况：没有要订阅的主题，预先订阅队列也满了，返回-1。*/
int CDataFlow::Subscribe(TopicID topic){
	//如果Topic==1,说明订阅的是网络流量信息
	if(SUBSCRIBE_MONITOR_INFO==topic){
		subMonitorInfo=true;
		pReceivingArray_->AddTopic(topic);
		return 1;
	}

	int i=0;
	MUTEX_LOCK(lock);
	for(i=0;i<tipSize;i++)
	{
		if(tip_[i]!=0 && topic==tip_[i]->topic_)
		{
			char tmpip[16];
			u_short tmpport;
			strcpy(tmpip , tip_[i]->ip);
			tmpport = tip_[i]->port;
			//存在要订阅的主题，调用CDataChannel进行订阅
			pReceivingArray_->AddTopic(topic);
			pDataChannel_->Subscribe(topic,tmpip,tmpport);
			cfi_->Subscribe(topic,tmpip,tmpport);
			MUTEX_UNLOCK(lock);
			return 1;
		}
	}
	MUTEX_UNLOCK(lock);
	//检查是否是本地发布的数据
	if(cfo_->TopicExist(topic)){
		cfo_->LocalSubscribe(topic);
			pReceivingArray_->AddTopic(topic);
			pSendingArray_->AddTopic(topic);
	}


	//走到这儿了就说明不存在这一主题，预先订阅一下
	MUTEX_LOCK(lock);
	printf("Preserve for topic %d\n: ", topic);
	for(i=0;i<tipSize;i++){
		if(preserve[i]==-1){
			preserve[i]=topic;
			break;
		}
	}
	MUTEX_UNLOCK(lock);

	//极端情况：没有要订阅的主题，预先订阅队列也满了
	if (i>=tipSize){
		return -1;
	}
	return 0;
}

/*由应用程序调用，解除自身主题的订阅,调用CDataChannel拆链，删除CDataFlow以及CReceivingArray中的结构*/
int CDataFlow::UnSubscribe(TopicID topic){

	if(topic==SUBSCRIBE_MONITOR_INFO){
		subMonitorInfo=false;
	}else{
		cfi_->Unsubscribe(topic);
		pDataChannel_->UnSubscribe(topic);
	}
	return pReceivingArray_->RemoveTopic(topic);
}

/*CDataChannel接收到来自TCP连接的订阅请求之后，调用该接口,在CSendingArray中的建立
Topic发送队列*/
int CDataFlow::RemoteSubscribe(TopicID topic){
	cfo_->Subscribe(topic);//在CFlowOut中保存订阅结构的信息用来统计。
	return 	pSendingArray_->AddTopic(topic);
}



/*CDataChannel调用，解除订阅报文,删除主题对应的队列，已经没有人订阅啦*/
int CDataFlow::RemoteUnsubscribe(TopicID topic){
	//cfo_->Unpublish(topic);
	return pSendingArray_->RemoveTopic(topic);
}

/*	1、完成组播Topic的功能
	2、完成组播监控数据的功能
	3、刷新本地保存的主题发布信息（引用周期性-1），
	并选择合适时机删除主题发布信息*/
void CDataFlow::MulticastAndRefresh(){
	//组播发布信息
	TopicID topics[MAX_TOPIC_SIZE];
	memset(topics,0,sizeof(topics));
	u_short i=0;
	cfo_->GetPublish(topics);
	for(i=0;i<MAX_TOPIC_SIZE;i++){
		if (topics[i]!=0){
				pDataChannel_->Publish(topics[i]);
		}
	}
	//组播监控信息1.FlowIn 2.FlouOut
	
	for (i=0;i<cfi_->tipSize;i++){
		if(cfi_->tip_[i]!=0){
			pDataChannel_->publishFlowIn(cfi_->tip_[i]->topic_,cfi_->tip_[i]->reference);
			cfi_->tip_[i]->reference=0;
		}
	}
	
	for(i=0;i<MAX_TOPIC_SIZE;i++){
		if (cfo_->tc_[i]!=0){
			pDataChannel_->publishFlowOut(cfo_->tc_[i]->topic_,cfo_->tc_[i]->sendCount_);
			cfo_->tc_[i]->sendCount_=0;
		}
	}

	//Refresh
	for(i=0;i<tipSize;i++){
		if(tip_[i]!=0){
			tip_[i]->reference--;
			if(tip_[i]->reference==0){
				delete tip_[i];
				tip_[i]=0;
			}
		}
	}
}

/*****************************************************************
1、	从CSendingArray中获取需要发送的数据提交给CDataChannel；
2、	周期性从CFlowOut中获取发布信息，利用CDataChannel发送到组播地址
3、	周期性刷新所保存的主题发布信息进行刷新（引用周期性-1），
	并选择合适时机删除主题发布信息
******************************************************************/
void CDataFlow::Run(){
	TopicID topic=-1;
	int ret=-1;
	time_t lastTime;//上一次组播的时间
	lastTime=time(NULL);//初始化
	char* buf=0;
	int size = 0;
	while (true)
	{
		buf = 0;
		//int none;
		ret=pSendingArray_->Receive(topic,buf,size);
		
		if (0==ret)
		{
			//超时退出 该组播和刷新本地tip结构了
			MulticastAndRefresh();
			lastTime=time(NULL);
		}
		else if(-1==ret)
		{
			printf("Error in CSendingArray Run!\n");
		}
		else
		{
			//有数据待发送，1.调用CDataChannel发送;2.调用CFlowOut统计;3.检查是否需要组播了
			pDataChannel_->Send(topic,buf,size);
			
			cfo_->Send(topic);
			if(cfo_->isLocalSubscribed(topic)){
				
				//如果存在本地订阅,直接发送到接收队列
				pReceivingArray_->Send(topic,buf,size);
			}
			if(time(NULL)-lastTime>MULTICAST_PERIOD){
				//该组播和刷新本地tip结构了
				MulticastAndRefresh();
				lastTime=time(NULL);
			}
				
		}
		if(buf) free(buf);
	}
}
/*接收来自CDataChannel的TCP数据，进而调用CReceivingArray的Send接口*/
int CDataFlow::Send(TopicID topic,char* buffer , int len){
	cfi_->Receive(topic);
	return pReceivingArray_->Send(topic,buffer,len);
}

//如果订阅了监视流数据
int CDataFlow::RemotePublishFlowInfo(char* rawData){
	if(subMonitorInfo){
		return pReceivingArray_->Send(SUBSCRIBE_MONITOR_INFO,rawData,UDP_HEADER_LEN);
	}
	return 0;
}