#ifndef CDATAFLOW_H
#define CDATAFLOW_H

class CDataFlow:public CThread,public CSender{

public:
	
	CDataFlow();

	~CDataFlow();
	
	int Assemble(CDataChannel* pDataChannel);

	int Assemble(CDataArray* pSendingArray , CDataArray* pReceivingArray);

	/******************************************************************
	 �����������ķ�������CFlowOut�н�����Ӧ�ṹ
	******************************************************************/
	int Publish(TopicID topic);
	
	/******************************************************************
	 ���������������Ķ���
	******************************************************************/
	int Subscribe(TopicID topic);
	
	/******************************************************************
	 �����������ķ���,ɾ��CFlowOut�н�����Ӧ�ṹ�Լ�CSendingArray�еĶ���
	******************************************************************/
	int UnPublish(TopicID topic);
	
	/******************************************************************
	 �����������Ķ���,ɾ��CDataFlow�Լ�CReceivingArray�еĽṹ
	******************************************************************/
	int UnSubscribe(TopicID topic);

	/******************************************************************
	 CDataChannel���յ�����TCP���ӵĶ�������֮�󣬵��øýӿ�
	******************************************************************/
	int RemoteSubscribe(TopicID topic);
	
	/*CDataChannel���յ��鲥��Ϣ�����*/
	int RemotePublish(TopicID topic , char* pAddr , unsigned short port);
	
	/*CDataChannel���ã�����������ģ���������Ҫ�Ķ���û�˷����ˣ�������Ԥ�ȶ��Ľṹ*/
	int RemoteUnpublish(TopicID topic);

	/*CDataChannel���ã�������ı���,ɾ�������Ӧ�Ķ���*/
	int RemoteUnsubscribe(TopicID topic);

	/******************************************************************
	��CDataChannel���ã���ɵײ����н������ݣ�TCP�����ϲ�Ĵ���
	******************************************************************/
	int Send(TopicID topic,char* buffer , int len);
	
	/*****************************************************************
	1��	��CSendingArray�л�ȡ��Ҫ���͵������ύ��CDataChannel��
	2��	�����Դ�CFlowOut�л�ȡ������Ϣ������CDataChannel���͵��鲥��ַ
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
	
	struct TopicIP** tip_;//�����Ϸ���������Topic��Ӧ��IPPort�ṹ
	int tipSize; //tip_�ṹ��ʵ�ʳ���
	//Ԥ�ȶ��Ľṹ
	int* preserve;

	bool subMonitorInfo;

	MUTEX lock;

};

#endif