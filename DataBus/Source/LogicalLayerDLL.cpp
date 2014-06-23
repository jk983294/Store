
#define DLL_EXPORT
#include "LogicalLayerDLL.h"

//入口函数 每次线程或者进程启动调用dll的时候调用
BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)

{
	
	switch (ul_reason_for_call){
		
	case DLL_PROCESS_ATTACH:
		PORT_ACCEPT=GetPortNumber();
		printf("dll被进程调用，得到端口号：%d\n",PORT_ACCEPT);

		printf("WSAStartup 初始化\n");
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
	printf("初始化CDataFlow,CSendingArray,CReceivingArray完成~\n");
	cno=new CNetworkOperator();
	
	//初始化 ASSEMBLE
	cno->Assemble(cdf,csa,cra);
	//启动CDataFlow
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