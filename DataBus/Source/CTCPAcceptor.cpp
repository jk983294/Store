#include "../include/DataBus.h"

CTCPAcceptor::CTCPAcceptor(CDataChannel* datachannel)
{
	datachannel_=datachannel;
	m_dbc_tcpacceptor_selfinterupt = false;
}

CTCPAcceptor::~CTCPAcceptor()
{
	closesocket(s_);
}
/************************************************************************
设定需要进行连接监听的地址和端口号；
两个输入参数，分别为IP地址和端口号；
无输出；                                                                     
************************************************************************/
void CTCPAcceptor::SetAddr(const char* ipaddr,unsigned short portNum)
{
	s_ = socket(AF_INET , SOCK_STREAM , IPPROTO_IP);
	if(s_ == SOCKET_ERROR)
	{
		printf("SOCEKT 初始化错误");
		return;
	}
			
			struct sockaddr_in localaddr;
			//设置本地地址族		
			localaddr.sin_addr.s_addr = inet_addr(ipaddr);//inet_addr(ipaddr)htonl(INADDR_ANY)
			localaddr.sin_family = AF_INET;
			localaddr.sin_port = htons(portNum);
			//socket option设置
			int nodelay = 1;
			if (setsockopt(s_, IPPROTO_TCP, TCP_NODELAY, (char *)&nodelay, sizeof(nodelay)) < 0)
			{
				printf("set	TCP opt Error!!!!!!!!!!\n");
			}
			int one = 1;

			if (setsockopt(s_, SOL_SOCKET, SO_REUSEADDR, (char *)&one, sizeof(one)) < 0)
			{
				printf("set	SOCKET opt SO_REUSEADDR Error!!!!!!!!!!\n");
			}
				
			if(bind(s_ , (struct sockaddr*)&localaddr , sizeof(localaddr)) == SOCKET_ERROR)
			{
						printf("set	SOCKET BIND Error!!!!!!!!!!\n");
						return;
			}
					//开始监听，最多8个连接
			listen(s_ , 8);
}
/************************************************************************/
/* 轮寻监听指定的网络地址和端口号；
如果有连接到达则建立对应的CSender和CReceiver对象，并提交给CDataChannel                                                                     */
/************************************************************************/
void CTCPAcceptor::Run()
{
	CSocket_utility* csu = new CSocket_utility();
	struct sockaddr_in clientaddr;
	while(1)
	{
		SOCKET sclient;

		int iaddrsize = sizeof(clientaddr);
	
		printf("TCP Acceptor Waiting for connection at socket:%d\n",s_);
		sclient = accept(s_ , (struct sockaddr*)&clientaddr , &iaddrsize);
		printf("accepted connection whose addr is: %s \n",inet_ntoa(clientaddr.sin_addr));
		
		/*交互接收对端的域名和主机名*/
		//接收初始化报文
		char tmpbuf[TCP_HEADER_LEN]; 
	
		printf("prepare to recv TCP Handshake packet\n");
		if(csu->recvFixedLengthData(sclient,tmpbuf,TCP_HEADER_LEN)!=TCP_HEADER_LEN)
		{
			printf("recv TCP Handshake packet failed ... \n");
		}
		short type ;
		char* paddr;
		u_short port;
		int topic;
		u_short len;
		csu->unpack_TCP_Header_packet(type,tmpbuf,topic,paddr,port,len);
		if (type == MSG_ESTABLISH)
		{	
			if((strcmp(paddr,datachannel_->multicast_host_addr)==0)&&port==0)
			{
				CSender* tcp_sender = new CTCPSender(sclient);
				CReceiver* tcp_receiver = new CTCPReceiver(sclient);  
				CPeer* ppeertemp = new CPeer(sclient,tcp_receiver,tcp_sender,
					inet_ntoa(clientaddr.sin_addr),clientaddr.sin_port);				
				datachannel_->setInterruptRecver(ppeertemp);
				printf("interrupt recver established...\n");
				m_dbc_tcpacceptor_selfinterupt = true;
				continue;
			}
			else
			{
				//TODO: 添加Peer
				CSender* tcp_sender = new CTCPSender(sclient);
				CReceiver* tcp_receiver = new CTCPReceiver(sclient);  
				//CPeer中记录对端的IP和监听端口号，
				//任何两个进程之间仅使用唯一的TCP连接进行通信
				//TCP连接的唯一性是通过监听端口号保证的
				CPeer* ppeertemp = new CPeer(sclient,tcp_receiver,tcp_sender,
					inet_ntoa(clientaddr.sin_addr),port); //port为握手报文中对端的监听端口
				/*
				 int ret = datachannel_->FindPeer(inet_ntoa(clientaddr.sin_addr),
					port);
				if(ret>=0)
				{
					//printf("ready to Remove Peer : %d\n" , ret);
					datachannel_->delPeerByIndex(ret);
					//printf("ready to stop data channel !!\n");
					datachannel_->Stop();
					//printf("ready to start data channel !!\n");
					datachannel_->Start();
					printf("data channel restarted!!\n");
				}
				*/
				datachannel_->Register(inet_ntoa(clientaddr.sin_addr),
				port,ppeertemp);				

				datachannel_->Interrupt_select();

			}			
		}
	}	
}

bool CTCPAcceptor::SelfInterupt()
{
	return m_dbc_tcpacceptor_selfinterupt;
}