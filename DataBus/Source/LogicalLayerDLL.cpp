
#define DLL_EXPORT
#include "LogicalLayerDLL.h"

//��ں��� ÿ���̻߳��߽�����������dll��ʱ�����
BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)

{
	
	switch (ul_reason_for_call){
		
	case DLL_PROCESS_ATTACH:
		PORT_ACCEPT=GetPortNumber();
		printf("dll�����̵��ã��õ��˿ںţ�%d\n",PORT_ACCEPT);

		printf("WSAStartup ��ʼ��\n");
		WSADATA  Ws;
		if ( WSAStartup(MAKEWORD(2,2), &Ws) != 0 )
		{
			printf("Init Windows Socket Failed::,%d\n",GetLastError);
			return -1;
		}

		break;
		
	case DLL_THREAD_ATTACH:break;
		
	case DLL_THREAD_DETACH:break;
		
	case DLL_PROCESS_DETACH:
		WSACleanup();
		break;
		
	}
	
	return TRUE;
	
}

void DllCall::Initialize(){
	cdf=new CDataFlow();
	csa=new CSendingArray();
	cra=new CReceivingArray();
	printf("��ʼ��CDataFlow,CSendingArray,CReceivingArray���~\n");
	cno=new CNetworkOperator();
	
	//��ʼ�� ASSEMBLE
	cno->Assemble(cdf,csa,cra);
	//����CDataFlow
	cdf->start();
}

int DllCall::Publish(TopicID topic){
return cdf->Publish(topic);
}
int  DllCall::Subscribe(TopicID topic){
return cdf->Subscribe(topic);
}

int  DllCall::Send(TopicID topic, char* data , int length){
return csa->Send(topic,data,length);
}
int  DllCall::Receive(TopicID& topic, char*& data , int& length){
return cra->Receive(topic,data,length);
}

int  DllCall::Unpublish(TopicID topic){
return cdf->UnPublish(topic);
}
int  DllCall::Unsubscribe(TopicID topic){
return cdf->UnSubscribe(topic);
}