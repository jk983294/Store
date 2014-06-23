#include "../include/DataBus.h"
/************************************************************************/
/* 构造函数                                                             */
/************************************************************************/
CNetworkOperator::CNetworkOperator()
{	
}
/************************************************************************/
/* 析构函数                                                             */
/************************************************************************/
CNetworkOperator::~CNetworkOperator(){
	delete datachannel;
	delete connector;
	delete acceptor;
}

int CNetworkOperator::Assemble(CDataFlow* cdf,CDataArray* cSendingArray,CDataArray* cReceivingArray)
{
	cdf->Assemble(cSendingArray,cReceivingArray);
	cdf->Assemble(datachannel);
	datachannel->Assemble(cdf);
	return 1;
}
/************************************************************************/
/* 完成对所有网络操作对象的生成和初始化.
需要生成的对象包括:
1、	如果探测到以太网接口则生成CTCPConnector、CTCPAcceptor；
2、	CDataChannel；
3、	用于组播的CUDPSender、CUDPReceiver；
初始化工作包括：
1、	对以太网接口进行初始化，获取本组件的端口号；
2、	建立CDataChannel与CDataFlow的关联；
3、	完成CUDPSender、CUDPReceiver在CDataChannel的注册；
4、	启动CTCPAcceptor；
5、	启动CDataChannel；                                                                     */
/************************************************************************/
int CNetworkOperator::Initialize(domain_id domainid)
{
	m_domainid = domainid;
	datachannel=new CDataChannel();
	datachannel->SetDomainID(domainid);
	connector=new CTCPConnector(datachannel);
	acceptor=new CTCPAcceptor(datachannel);
	m_netmonitor = new CNetMonitor();
	char  name[255];
	PHOSTENT  hostinfo;
	char* ip;
	if(  gethostname  (  name,  sizeof(name))  ==  0)  
	{  
		if((hostinfo  =  gethostbyname(name))  !=  NULL)  
		{  
			ip  =  inet_ntoa  (*(struct  in_addr  *)*hostinfo->h_addr_list);  
		}  
	} 
	int err=GetLastError();
	printf("host name:%s  host ip:%s host port:%d\n",name,ip , PORT_ACCEPT);
	datachannel->setMulticastparameters(PORT_ACCEPT,ip);
	acceptor->SetAddr(datachannel->multicast_host_addr,PORT_ACCEPT);
	acceptor->Start();
	m_netmonitor->Assemble(datachannel);
	m_netmonitor->Start();
	
	interrupt_peer_init();
	multicast_peer_init();

	datachannel->setConnector(connector);
	
	while(1)
	{
		if(((CTCPAcceptor*)acceptor)->SelfInterupt())
		{
			break;
		}
		else
		{
			printf("监听端口初始化\n");
			Sleep(500);
		}
	}
	printf("顺利跑到这里。3\n");
	datachannel->Start();
	return 1;
}
int CNetworkOperator::interrupt_peer_init(){
	if(((CTCPConnector*)connector)->Connect(1,datachannel->multicast_host_addr,0))
		return 1;
	else{
		perror("interrupt_peer_init error");
		return -1;
	}
}
int CNetworkOperator::multicast_peer_init()
{
	//create UDP sender     
    SOCKET udpsender_;        
    /* create what looks like an ordinary UDP socket */    
    if ((udpsender_=socket(AF_INET,SOCK_DGRAM,0))==0)     
    {    
        perror("udp socket init error");    
        return -1;    
    }   
	int optval = 2;
	if (setsockopt(udpsender_, IPPROTO_IP, IP_MULTICAST_TTL, (char *)&optval, sizeof(int)) == SOCKET_ERROR)
	{
		perror("udp socket setsocketopt TTL error");
	}    
	CSender* udp_sender = new CUDPSender(udpsender_);
	
	
	//create UDP receiver 
    SOCKET udpreceiver_;            
    if ((udpreceiver_=socket(AF_INET,SOCK_DGRAM,0)) < 0)     
    {    
        perror("udp receiver socket create error");    
        return -1;    
    } 
	struct sockaddr_in addr;   	
    struct ip_mreq mreq;       
    u_int yes=1;   
	/**** MODIFICATION TO ORIGINAL */    
    /* allow multiple sockets to use the same PORT number */    
    if (setsockopt(udpreceiver_,SOL_SOCKET,SO_REUSEADDR,(char *)&yes,sizeof(yes)) < 0)     
    {    
        perror("UDP Reusing ADDR failed"); 
		return -1;
    }    
    /*** END OF MODIFICATION TO ORIGINAL */    
    /* set up destination address */    
    memset(&addr,0,sizeof(addr));    
    addr.sin_family=AF_INET;    
    addr.sin_addr.s_addr=htonl(INADDR_ANY); /* N.B.: differs from sender */    
    addr.sin_port=htons(MULTICAST_PORT);    
    /* bind to receive address */    
    if (bind(udpreceiver_,(struct sockaddr *) &addr,sizeof(addr)) < 0)    
    {    
        perror("udp multicast bind error");    
        return -1;    
    }    
    /* use setsockopt() to request that the kernel join a multicast group */    
    mreq.imr_multiaddr.s_addr=inet_addr(MULTICAST_GROUP);    
    mreq.imr_interface.s_addr=htonl(INADDR_ANY);    
    if (setsockopt(udpreceiver_,IPPROTO_IP,IP_ADD_MEMBERSHIP,(char *)&mreq,sizeof(mreq)) < 0)     
    {    
        perror("udp multicast setsockopt join a multicast group error");  
		DWORD err = GetLastError();
        return -1;    
    }
	printf("udp recv socket at: %d\n",udpreceiver_);
    CReceiver* udp_receiver = new CUDPReceiver(udpreceiver_);  
	CPeer* ppeertemp = new CPeer(udpreceiver_,udp_receiver,udp_sender,NULL,0);
	datachannel->setMulticastPeer(ppeertemp);
	return 1;
}
void CNetworkOperator::Destroy(){

}