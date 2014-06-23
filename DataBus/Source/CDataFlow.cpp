#include "../include/DataBus.h"

CDataFlow::CDataFlow(){
	cfo_=new CFlowOut();
	cfi_=new CFlowIn();
	//���ñ䳤�����ݽṹ������������չ
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

/*�����������ķ�������CFlowOut�н�����Ӧ�ṹ*/
int  CDataFlow::Publish(TopicID topic){
	cfo_->Publish(topic);
	//���Ԥ�ȶ��Ľṹ
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

/*�����������ķ���,����CDataChannel�����·��ɾ��CFlowOut�н�����Ӧ�ṹ�Լ�CSendingArray�еĶ���*/
int CDataFlow::UnPublish(TopicID topic)
{
	cfo_->Unpublish(topic);
	pDataChannel_->UnPublish(topic);
	return pSendingArray_->RemoveTopic(topic);
}

/*CDataChannel���յ������鲥�����ⷢ����Ϣ֮����øýӿ�
1.ά��tip�ṹ
2.����Ƿ���Ԥ�ȶ���ṹ
���������ģ�CFlowIn�н����ṹ������CDataChannel��Subscribe��
����ֵ��-1��tip�ṹ����;
		0,���Լ����鲥��ȥ��topic��ʲô������
*/
int CDataFlow::RemotePublish(TopicID topic , char* pAddr , unsigned short port){
	//printf("�յ��鲥����:%d\n",topic);
	//�ж��ǲ����Լ��鲥��ȥ��
	if(cfo_->TopicExist(topic)){
		return 0;
	}
	u_short i=0;
	MUTEX_LOCK(lock);
	for(i=0;i<tipSize;i++){
		if(tip_[i]!=0 && tip_[i]->topic_==topic){
			//�Ѿ������˸�topic������
			strcpy(tip_[i]->ip,pAddr);
			tip_[i]->port=port;
			tip_[i]->reference++;//����������+1
			break;
		}
	}
	
	if(i==tipSize)
	{
		//û�ҵ�,˵����һ�ν��յ���topic�ķ�����Ϣ
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
	
	if(i==tipSize){//���ˡ���
		printf("Topic Over Sized in CDataFlow.RemotePublish!");
		return -1;
	}
	
	//�������˼���Ƿ���Ԥ�ȶ��Ľṹ
	for(i=0;i<tipSize;i++){
		if(preserve[i]==topic){
			//����Ҫ���ĵ����⣬����CDataChannel���ж���
			pReceivingArray_->AddTopic(topic);
			pDataChannel_->Subscribe(topic,pAddr,port);
			cfi_->Subscribe(topic,pAddr,port);
			MUTEX_LOCK(lock);
			preserve[i]=-1;//������Ҫע��Ԥ�ȶ��Ľṹ
			MUTEX_UNLOCK(lock);
			break;
		}
	}
	return 1;
}
/*CDataChannel���յ�����TCP���ӵĽ����������ʱ��
����CDataFlow��RemoteUnPublish�ӿڡ�
CDataFlowɾ���������ϵͳ����������Ϣ��CFlowIn�е������ӵ�ϵͳ����������Ϣ
ͬʱ����һ��Ԥ�ȶ��Ľṹ��
��������CReceivingArray��RemoveTopicɾ�������Ӧ�Ķ���*/
int CDataFlow::RemoteUnpublish(TopicID topic)
{
	int index = 0;
	MUTEX_LOCK(lock);
	for (index=0;index<tipSize;index++)	{
		if(tip_[index]!=0 && tip_[index]->topic_==topic){
			delete tip_[index];//�������ͷ��ڴ�
			tip_[index]=0;
			break;
		}
	}
	//Ԥ����
	for(index=0;index<tipSize;index++){
		if(preserve[index]==-1){
			preserve[index]=topic;
			break;
		}
	}
	MUTEX_UNLOCK(lock);
	
	//ɾ���Ѿ����������ӵ�ϵͳ����������Ϣ
	cfi_->Unsubscribe(topic);
	
	return pReceivingArray_->RemoveTopic(topic);

}

/*���������������Ķ��ģ����tip_����û�и�topic��
���У��������ģ�CFlowIn�н����ṹ������CDataChannel��Subscribe��������1��
��û�У�����Ԥ�ȶ��Ļ���������0��
���������û��Ҫ���ĵ����⣬Ԥ�ȶ��Ķ���Ҳ���ˣ�����-1��*/
int CDataFlow::Subscribe(TopicID topic){
	//���Topic==1,˵�����ĵ�������������Ϣ
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
			//����Ҫ���ĵ����⣬����CDataChannel���ж���
			pReceivingArray_->AddTopic(topic);
			pDataChannel_->Subscribe(topic,tmpip,tmpport);
			cfi_->Subscribe(topic,tmpip,tmpport);
			MUTEX_UNLOCK(lock);
			return 1;
		}
	}
	MUTEX_UNLOCK(lock);
	//����Ƿ��Ǳ��ط���������
	if(cfo_->TopicExist(topic)){
		cfo_->LocalSubscribe(topic);
			pReceivingArray_->AddTopic(topic);
			pSendingArray_->AddTopic(topic);
	}


	//�ߵ�����˾�˵����������һ���⣬Ԥ�ȶ���һ��
	MUTEX_LOCK(lock);
	printf("Preserve for topic %d\n: ", topic);
	for(i=0;i<tipSize;i++){
		if(preserve[i]==-1){
			preserve[i]=topic;
			break;
		}
	}
	MUTEX_UNLOCK(lock);

	//���������û��Ҫ���ĵ����⣬Ԥ�ȶ��Ķ���Ҳ����
	if (i>=tipSize){
		return -1;
	}
	return 0;
}

/*��Ӧ�ó�����ã������������Ķ���,����CDataChannel������ɾ��CDataFlow�Լ�CReceivingArray�еĽṹ*/
int CDataFlow::UnSubscribe(TopicID topic){

	if(topic==SUBSCRIBE_MONITOR_INFO){
		subMonitorInfo=false;
	}else{
		cfi_->Unsubscribe(topic);
		pDataChannel_->UnSubscribe(topic);
	}
	return pReceivingArray_->RemoveTopic(topic);
}

/*CDataChannel���յ�����TCP���ӵĶ�������֮�󣬵��øýӿ�,��CSendingArray�еĽ���
Topic���Ͷ���*/
int CDataFlow::RemoteSubscribe(TopicID topic){
	cfo_->Subscribe(topic);//��CFlowOut�б��涩�Ľṹ����Ϣ����ͳ�ơ�
	return 	pSendingArray_->AddTopic(topic);
}



/*CDataChannel���ã�������ı���,ɾ�������Ӧ�Ķ��У��Ѿ�û���˶�����*/
int CDataFlow::RemoteUnsubscribe(TopicID topic){
	//cfo_->Unpublish(topic);
	return pSendingArray_->RemoveTopic(topic);
}

/*	1������鲥Topic�Ĺ���
	2������鲥������ݵĹ���
	3��ˢ�±��ر�������ⷢ����Ϣ������������-1����
	��ѡ�����ʱ��ɾ�����ⷢ����Ϣ*/
void CDataFlow::MulticastAndRefresh(){
	//�鲥������Ϣ
	TopicID topics[MAX_TOPIC_SIZE];
	memset(topics,0,sizeof(topics));
	u_short i=0;
	cfo_->GetPublish(topics);
	for(i=0;i<MAX_TOPIC_SIZE;i++){
		if (topics[i]!=0){
				pDataChannel_->Publish(topics[i]);
		}
	}
	//�鲥�����Ϣ1.FlowIn 2.FlouOut
	
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
1��	��CSendingArray�л�ȡ��Ҫ���͵������ύ��CDataChannel��
2��	�����Դ�CFlowOut�л�ȡ������Ϣ������CDataChannel���͵��鲥��ַ
3��	������ˢ������������ⷢ����Ϣ����ˢ�£�����������-1����
	��ѡ�����ʱ��ɾ�����ⷢ����Ϣ
******************************************************************/
void CDataFlow::Run(){
	TopicID topic=-1;
	int ret=-1;
	time_t lastTime;//��һ���鲥��ʱ��
	lastTime=time(NULL);//��ʼ��
	char* buf=0;
	int size = 0;
	while (true)
	{
		buf = 0;
		//int none;
		ret=pSendingArray_->Receive(topic,buf,size);
		
		if (0==ret)
		{
			//��ʱ�˳� ���鲥��ˢ�±���tip�ṹ��
			MulticastAndRefresh();
			lastTime=time(NULL);
		}
		else if(-1==ret)
		{
			printf("Error in CSendingArray Run!\n");
		}
		else
		{
			//�����ݴ����ͣ�1.����CDataChannel����;2.����CFlowOutͳ��;3.����Ƿ���Ҫ�鲥��
			pDataChannel_->Send(topic,buf,size);
			
			cfo_->Send(topic);
			if(cfo_->isLocalSubscribed(topic)){
				
				//������ڱ��ض���,ֱ�ӷ��͵����ն���
				pReceivingArray_->Send(topic,buf,size);
			}
			if(time(NULL)-lastTime>MULTICAST_PERIOD){
				//���鲥��ˢ�±���tip�ṹ��
				MulticastAndRefresh();
				lastTime=time(NULL);
			}
				
		}
		if(buf) free(buf);
	}
}
/*��������CDataChannel��TCP���ݣ���������CReceivingArray��Send�ӿ�*/
int CDataFlow::Send(TopicID topic,char* buffer , int len){
	cfi_->Receive(topic);
	return pReceivingArray_->Send(topic,buffer,len);
}

//��������˼���������
int CDataFlow::RemotePublishFlowInfo(char* rawData){
	if(subMonitorInfo){
		return pReceivingArray_->Send(SUBSCRIBE_MONITOR_INFO,rawData,UDP_HEADER_LEN);
	}
	return 0;
}