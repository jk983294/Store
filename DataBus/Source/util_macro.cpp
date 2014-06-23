#define DLL_EXPORT
#include "util_macro.h"

#pragma data_seg("shared")
//进程间共享的Port号的定义
u_short _globalPort=DEFAULT_PORT;

HANDLE mutex=CreateMutex(0, FALSE, 0);
#pragma data_seg()
#pragma comment(linker,"/SECTION:shared,RWS")

//进程内使用的PORT_ACCEPT
u_short PORT_ACCEPT=DEFAULT_PORT;

void SetPortNumber(u_short port){
	PORT_ACCEPT=port;
}

u_short GetPortNumber(){
	int ret;
	WaitForSingleObject(mutex,INFINITE);
	_globalPort++;
	ret=_globalPort;
	ReleaseMutex(mutex);
	return ret;
}
