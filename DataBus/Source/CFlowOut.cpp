#include "../include/DataBus.h"

CFlowOut::CFlowOut(){
	for(u_short i=0;i<MAX_TOPIC_SIZE;i++){
		tc_[i]=0;
	}
}

CFlowOut::~CFlowOut(){
	for(u_short i=0;i<MAX_TOPIC_SIZE;i++){
		if (tc_[i]!=0)
		{
			delete tc_[i];
		}	
	}
	
}
/*�������ķ�����Ϣ����� �Ѿ����ڷ���0��
��ӳɹ�����1��
ʧ�ܷ���-1���ﵽMAX_TOPIC_SIZE*/
int CFlowOut::Publish(TopicID topic){
	int ret=-1,i=0;
	if (TopicExist(topic)){
		return 0;
	}
	for(i=0;i<MAX_TOPIC_SIZE;i++){
		if (tc_[i]==0){
			tc_[i]=new struct TopicCount;;
			tc_[i]->topic_=topic;
			tc_[i]->subCount_=0;
			tc_[i]->localSub=false;
			ret=1;
			break;
		}	
	}
	if(ret<0){
		printf("CFlowOut have reached the max topic count!\n");
	}
	return ret;
}

int CFlowOut::Unpublish(TopicID topic){
	int i=0;
	for (i=0;i<MAX_TOPIC_SIZE;i++){
		if(tc_[i]!=0 && tc_[i]->topic_==topic){
			delete tc_[i];
			tc_[i]=0;
			break;
		}
	}
	if (i>=MAX_TOPIC_SIZE){
		return -1; //û�з���Topic��TopicCount�ṹ
	}
	return 1;
}

/*ͳ�Ʒ��ʹ���*/
int  CFlowOut::Send(TopicID topic){
	int ret=-1;
	for(u_short i=0;i<MAX_TOPIC_SIZE;i++){
		if(tc_[i]!=0 && tc_[i]->topic_==topic ){
			tc_[i]->sendCount_++;
			ret=1;
			break;
		}
	}
	return ret;
}
//����Topic�ĵı����Ĵ���+1
int CFlowOut::Subscribe(TopicID topic){
	int ret=-1;
	for(u_short i=0;i<MAX_TOPIC_SIZE;i++){
		if(tc_[i]!=0 && tc_[i]->topic_==topic ){
			tc_[i]->subCount_++;
			ret=1;
			break;
		}
	}
	return ret;
}
//����Է�����
int CFlowOut::LocalSubscribe(TopicID topic){
	int ret=-1;
	for(u_short i=0;i<MAX_TOPIC_SIZE;i++){
		if(tc_[i]!=0 && tc_[i]->topic_==topic ){
			tc_[i]->subCount_++;
			tc_[i]->localSub=true;
			ret=1;
			break;
		}
	}
	return ret;
}

//����Topic�ĵı����Ĵ���-1
int CFlowOut::UnSubscribe(TopicID topic){
	int ret=-1;
	for(u_short i=0;i<MAX_TOPIC_SIZE;i++){
		if(tc_[i]!=0 && tc_[i]->topic_==topic ){
			tc_[i]->subCount_--;
			ret=1;
			break;
		}
	}
	return ret;
}
/*���Ҫ�鲥��TOPIC����*/
void CFlowOut::GetPublish(TopicID topics[]){
	for (u_short i=0;i<MAX_TOPIC_SIZE;i++){
		topics[i]=(tc_[i]!=0) ? tc_[i]->topic_ : 0;
	}
}

bool CFlowOut::TopicExist(TopicID topic){
	bool flag=false;
	for(u_short i=0;i<MAX_TOPIC_SIZE;i++){
		if(tc_[i]!=0&&tc_[i]->topic_==topic){
			flag=true;
			break;
		}
	}
	return flag;
}
bool CFlowOut::isLocalSubscribed(TopicID topic){
	bool flag=false;
	for(u_short i=0;i<MAX_TOPIC_SIZE;i++){
		if(tc_[i]!=0 && tc_[i]->topic_==topic){
			flag=tc_[i]->localSub;
			break;
		}
	}
	return flag;
}
