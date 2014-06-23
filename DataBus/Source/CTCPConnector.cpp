#include "../include/DataBus.h"

CTCPConnector::CTCPConnector(CDataChannel* datachannel){
	tcpheader_=new char[TCP_HEADER_LEN];
	datachannel_=datachannel;
}
CTCPConnector::~CTCPConnector()
{
	delete[] tcpheader_;
	closesocket(s_);
}
/**************************************************	
方法名称：connect_S
输入参数：
ipaddr：ip地址,portNum:端口号 type: 1:select中断连接 0：正常TCP连接
输出参数：SOCKET NUMBER if successful. 0 if failed。
返回值： socket句柄。
函数说明：此函数发送服务属性。
**************************************************/
SOCKET CTCPConnector::Connect(short type,const char* ipaddr,unsigned short portNum){
	//lack of version negotiation

	s_=0;
	printf("connector begin connect to %s...\n",ipaddr);
	s_=socket(AF_INET , SOCK_STREAM , IPPROTO_TCP);
	sockaddr_in addr;
	sockaddr_in peeraddr;
	
	addr.sin_family = AF_INET;
	addr.sin_port = 0;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	int nodelay = 1; //为什么要设置nodelay？
	if (setsockopt(s_, IPPROTO_TCP, TCP_NODELAY, (char *)&nodelay, sizeof(nodelay)) < 0)
	{
		printf("set	TCP opt Error!!!!!!!!!!\n");
		return 0;
	}
	int one = 1;//设置重用端口？
	if (setsockopt(s_, SOL_SOCKET, SO_REUSEADDR, (char *)&one, sizeof(one)) < 0)
	{
		printf("set	SOCKET opt SO_REUSEADDR Error!!!!!!!!!!\n");
		return 0;
	}
	
	bind(s_ , (struct sockaddr*)&addr , sizeof(addr));
	
	//设置对端
	peeraddr.sin_family = AF_INET;
	if (portNum==0)
	{
		peeraddr.sin_port = htons(PORT_ACCEPT);
	}
	else{
		peeraddr.sin_port = htons(portNum);
	}
	peeraddr.sin_addr.s_addr = inet_addr(ipaddr);
	
	//超时重连
	for(u_short retrytimes = 0; retrytimes<3; retrytimes++)
	{

		if(connect(s_ , (struct sockaddr*)&peeraddr , sizeof(peeraddr)) == SOCKET_ERROR)
		{
			int err = GetLastError();
			printf("Connect Error : %d !\n" , err);

			return 0;
		}
		else
		{
			break;
		}
	}

	CSocket_utility* csu= new CSocket_utility();
	char *addrtmp = NULL;
	addrtmp = new char[64];
	if (type ==1)
	{	 		
		strcpy(addrtmp,datachannel_->multicast_host_addr);
		printf("select interupt handshake: %s \n",addrtmp);
		csu->pack_TCP_Header_packet(tcpheader_,MSG_ESTABLISH,0,addrtmp,0,0);
	}
	else {
		strcpy(addrtmp,ipaddr);
		printf("cpmmon tcp link handshake:%s \n",addrtmp);
		//csu->pack_TCP_Header_packet(tcpheader_,MSG_ESTABLISH,0,addrtmp,portNum,0);
		csu->pack_TCP_Header_packet(tcpheader_,MSG_ESTABLISH,0,addrtmp,PORT_ACCEPT,0);
	}
	delete[] addrtmp;

	//TODO: 添加Peer
	CSender* tcp_sender = new CTCPSender(s_);
	CReceiver* tcp_receiver = new CTCPReceiver(s_); 
	if (type ==1)
	{
		CPeer* ppeertemp = new CPeer(s_,tcp_receiver,tcp_sender,
			inet_ntoa(peeraddr.sin_addr),peeraddr.sin_port);
		datachannel_->setInterruptSender(ppeertemp);
		printf("interrupt sender established...\n");
	}
	else if (type==0)
	{
		CPeer* ppeertemp = new CPeer(s_,tcp_receiver,tcp_sender,
			inet_ntoa(peeraddr.sin_addr),portNum);
		datachannel_->Register(inet_ntoa(peeraddr.sin_addr),
			portNum,ppeertemp);
		datachannel_->Interrupt_select();
		printf("connector register peer into datachannel success...\n");
	}

	printf("send TCP handshake packet\n");
	if(!(csu->sendFixedLengthData(s_,tcpheader_,TCP_HEADER_LEN))){
		printf("send TCP handshake packet failure...\n");
	}
	
// 	if (tmpbuf!=NULL)
// 		delete[] tmpbuf;
	return s_;
}

SOCKET CTCPConnector::DisConnect(const char* ipaddr,unsigned short portNum){
	s_=0;
	//从DataChannel取得Peer，然后取得SocketID，Remove&Close
	return s_;
}
