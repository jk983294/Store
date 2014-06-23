#include "../include/DataBus.h"

CFlowIn::CFlowIn(){
	tipSize=MAX_TOPIC_SIZE;
	tip_= new TopicIP*[tipSize];
	for(u_short i=0;i<tipSize;i++){
		tip_[i]=0;
	}
}
CFlowIn::~CFlowIn(){
	for(u_short i=0;i<tipSize;i++){
		if(tip_[i]!=0)
			delete tip_[i];
	}
	delete[] tip_;
}


int CFlowIn::Publish(TopicID topic ){
	
return 0;
}

int CFlowIn::Unsubscribe(TopicID topic){
	int i=0;
	for (i=0;i<tipSize;i++){
		if(tip_[i]!=0 && tip_[i]->topic_==topic){
			delete tip_[i];
			tip_[i]=0;
			break;
		}
	}
	if (i>=tipSize){
		return -1; //没有符合Topic的TopicCount结构
	}
	return 1;
}

/*添加连接，建立结构*/
int CFlowIn::Subscribe(TopicID topic , char* pAddr , unsigned short port){
	int i=0,index=-1;
	for(i=0;i<tipSize;i++){
		if(tip_[i]!=0 && tip_[i]->topic_==topic){
			//已经存在了该topic，更新
			index=i;
			strcpy(tip_[i]->ip,pAddr);
			tip_[i]->port=port;
	//		tip_[i]->reference++;//引用周期性+1
			break;
		}
	}
	
	if(index<0){
		//没找到,新建
		for (i=0;i<tipSize;i++){
			if (tip_[i]==0){
				tip_[i]=new struct TopicIP;
				tip_[i]->topic_=topic;
				strcpy(tip_[i]->ip,pAddr);
				tip_[i]->port=port;
				//tip_[i]->reference =1;
				break;
			}
		}
	}else{
		return 0;
	}
	if(i==tipSize){//满了……
		printf("Topic Over Sized in CFlowIn!");
		return -1;
	}
	return 1;
}
int CFlowIn::Receive(TopicID topic){
	for(u_short i=0;i<tipSize;i++){
		if(tip_[i]!=0 && tip_[i]->topic_==topic){
			tip_[i]->reference++;
			return 1;
		}
	}
	return -1;
}
