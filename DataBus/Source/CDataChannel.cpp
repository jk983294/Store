#include "../include/DataBus.h"

/**************************************************	
方法名称：ChannelManager
**************************************************/
CDataChannel::CDataChannel()
{
	u_short index = 0;
	tcpheader_ = new char[TCP_HEADER_LEN];
	udpheader_=new char [UDP_HEADER_LEN];
	csu = new CSocket_utility();
	for(index=0;index<MAXPEERNUM;index++)
		m_peers[index] = NULL;

	MUTEX_INIT(peer_lock_);

	for(index=0;index<TOPIC_MAXNUM;index++)
		for (int j=0;j<PEER_MAXNUM_FOR_EACH_TOPIC;j++)
			m_topicmapping[index][j]=-1;
}

/**************************************************	
方法名称：~ChannelManager
**************************************************/
CDataChannel::~CDataChannel()
{
	delete[] tcpheader_;
	delete[] udpheader_;
	for(u_short i=0;i<MAXPEERNUM;i++)
	{
		if(m_peers[i]!=0)
		{
			delete m_peers[i];
		}
		
	}
	MUTEX_DESTROY(peer_lock_);
	if (!multicast_peer)
		delete multicast_peer;
	if (!interrupt_sender)
		delete interrupt_sender;
	if (!interrupt_recver)
		delete interrupt_recver;
}	



/*
CDataFlow调用该接口向网络发送数据
三个输入参数：分别为数据主题、需要发送的数据存储区域的指针和发送数据的长度；
一个输出参数：成功则返回1，否则返回错误代码；
；
*/
int CDataChannel::Send(int topic, char* data , int length)
{
/*	char* tcpheader = */
	MUTEX_LOCK(peer_lock_);
	u_short i,j;
	for (i=0;i<TOPIC_MAXNUM;i++)
	{
		if (topic == m_topicmapping[i][0])
		{
			for (j=1;j<PEER_MAXNUM_FOR_EACH_TOPIC;j++)
			{
				if (m_topicmapping[i][j] != -1)
				{
					if (m_peers[m_topicmapping[i][j]]!=NULL)
					{
						int ret = 0;
						csu->pack_TCP_Header_packet(tcpheader_,MSG_DATA_TCP,topic,NULL,0,length);
						ret = m_peers[m_topicmapping[i][j]]->sendData(tcpheader_,TCP_HEADER_LEN);
						ret = m_peers[m_topicmapping[i][j]]->sendData(data,length);
						if(ret == -1)
						{
							printf("socket %d send error\n");
						}
						else
						{
							printf("topic %d data sended to socket:%d\n",topic,m_peers[m_topicmapping[i][j]]->s_);
						}
					}	
				}
			}
			break;
		}		
	}
	MUTEX_UNLOCK(peer_lock_);
	return 1;
}

/*将生成的CPeer加入
两个输入参数：对端IP和CPeer实例；
一个输出参数：一个输出参数：成功则返回1，否则返回错误代码；
*/
int CDataChannel::Register(char* pAddr , unsigned short port ,CPeer* pPeer)
{
	u_short index;
	pPeer->reading_ = 0;
	MUTEX_LOCK(peer_lock_);
	for(index=0;index<MAXPEERNUM;index++)
	{
		if(m_peers[index] != 0)
		{
			if((!strcmp(m_peers[index]->pAddr , pAddr)) && (m_peers[index]->port == port))
			{
				printf("Find Existed Peer! %s , %d\n" , m_peers[index]->pAddr , m_peers[index]->port);
				
				for (int index2=0;index2<MAX_TOPIC_SIZE;index2++)
				{
					if (m_peers[index]->subscribe_topicID[index2]!=-1)
					{
						printf("Delete Peer , so do RemoteUnPublish\n");
						cdf->RemoteUnpublish(m_peers[index]->subscribe_topicID[index2]);
					}
				}
				TopicID topic;
				delete m_peers[index];
				m_peers[index]=NULL;
				
				int i,j,count;
				for (i=0;i<TOPIC_MAXNUM;i++)
				{
					if (m_topicmapping[i][0] != -1 )
					{
						count=0;
						for (j=1;j<PEER_MAXNUM_FOR_EACH_TOPIC;j++)
						{
							if (m_topicmapping[i][j] == index)
							{
								topic = m_topicmapping[i][0];
								m_topicmapping[i][j] = -1;
								printf("peer %d removed in queue topic %d ...\n",index,m_topicmapping[i][0]);
							}
							else if (m_topicmapping[i][j] != -1)
								count++;
						}
						if (count==0)//该种topic无人订阅
						{
							printf("没有组件订阅该数据！\n");
							cdf->RemoteUnsubscribe(topic);
							m_topicmapping[i][0] = -1;
						}
					}		
				}
				
				m_peers[index] = pPeer;
				MUTEX_UNLOCK(peer_lock_);
				return 1;
			}
		}
	}
	for(index=0;index<MAXPEERNUM;index++)
	{
		if(m_peers[index] == 0)
		{
			m_peers[index] = pPeer;
			MUTEX_UNLOCK(peer_lock_);
			return 1;
		}
	}
	MUTEX_UNLOCK(peer_lock_);
	return -1;

}
int CDataChannel::delPeerByIndex(int index)
{
	MUTEX_LOCK(peer_lock_);
	TopicID topic;
	delete m_peers[index];
	m_peers[index]=NULL;
	int i,j,count;
	for (i=0;i<TOPIC_MAXNUM;i++)
	{
		if (m_topicmapping[i][0] != -1 )
		{
			count=0;
			for (j=1;j<PEER_MAXNUM_FOR_EACH_TOPIC;j++)
			{
				if (m_topicmapping[i][j] == index)
				{
					topic = m_topicmapping[i][0];
					m_topicmapping[i][j] = -1;
					printf("peer %d removed in queue topic %d ...\n",index,m_topicmapping[i][0]);
				}
				else if (m_topicmapping[i][j] != -1)
					count++;
			}
			if (count==0)//该种topic无人订阅
			{
				printf("没有组件订阅该数据！\n");
				cdf->RemoteUnsubscribe(topic);
				m_topicmapping[i][0] = -1;
			}
		}		
	}
	MUTEX_UNLOCK(peer_lock_);
	return 1;

}
/*将已有的CPeer剥离
两个输入参数：对端IP和CPeer实例引用；
一个输出参数：一个输出参数：成功则返回1，否则返回错误代码；
*/
int CDataChannel::UnRegister(CPeer* pPeer){
	return -1;
}

int CDataChannel::FindPeer(char* pAddr , unsigned short port)
{

	MUTEX_LOCK(peer_lock_);
	u_short index = 0 , index2 = 0;
	for(index=0;index<MAXPEERNUM;index++)
	{
		if(m_peers[index] != 0)
		{
			if((!strcmp(m_peers[index]->pAddr , pAddr)) && (m_peers[index]->port == port) && (m_peers[index]->reading_ == 1))
			{
				printf("Find Existed Peer! %s , %d\n" , m_peers[index]->pAddr , m_peers[index]->port);
				for (index2=0;index2<MAX_TOPIC_SIZE;index2++)
				{
					if (m_peers[index]->subscribe_topicID[index2]!=-1)
					{
						printf("Delete Peer , so do RemoteUnPublish\n");
						cdf->RemoteUnpublish(m_peers[index]->subscribe_topicID[index2]);
					}
				}
				MUTEX_UNLOCK(peer_lock_);
				return index;
			}
		}
	}
	MUTEX_UNLOCK(peer_lock_);
	return -1;
}

int CDataChannel::TestPeers()
{
	u_short index=0 , index2 = 0 , index3 = 0 , i , j , count;
	int findinvalid = 0;
	TopicID topic;
	MUTEX_LOCK(peer_lock_);
	for(index=0;index<MAXPEERNUM;index++)
	{
		if(m_peers[index] != 0)
		{
			if(m_peers[index]->reading_ && m_peers[index]->valid_)
			{
				time_t tmptime = time(NULL);
				if(tmptime - m_peers[index]->time_ > 3)
				{
					m_peers[index]->valid_ = 0;
					printf("Find Invalid Peer %d , UnPublish\n" , index);
					for (index2=0;index2<MAX_TOPIC_SIZE;index2++)
					{
						if (m_peers[index]->subscribe_topicID[index2]!=-1)
						{
							cdf->RemoteUnpublish(m_peers[index]->subscribe_topicID[index2]);
						}
					}
					
					for (i=0;i<TOPIC_MAXNUM;i++)
					{
						if (m_topicmapping[i][0] != -1 )
						{
							count=0;
							for (j=1;j<PEER_MAXNUM_FOR_EACH_TOPIC;j++)
							{
								if (m_topicmapping[i][j] == index)
								{
									topic = m_topicmapping[i][0];
									m_topicmapping[i][j] = -1;
									printf("peer %d removed in queue topic %d ...\n",index,m_topicmapping[i][0]);
								}
								else if (m_topicmapping[i][j] != -1)
									count++;
							}
							if (count==0)//该种topic无人订阅
							{
								printf("没有组件订阅该数据！\n");
								cdf->RemoteUnsubscribe(topic);
								m_topicmapping[i][0] = -1;
							}
						}		
					}
					findinvalid = 1;
					
					for(index3=0;index3<MAXPEERNUM;index3++)
					{
						if(m_peers[index3] != 0)
						{
							if((!strcmp(m_peers[index3]->pAddr , m_peers[index]->pAddr)) && 
								(m_peers[index3]->port == m_peers[index]->port) && index!=index3)
							{
								
								printf("Find Invalid Peer %d , UnPublish\n" , index3);
								for (index2=0;index2<MAX_TOPIC_SIZE;index2++)
								{
									if (m_peers[index3]->subscribe_topicID[index2]!=-1)
									{
										cdf->RemoteUnpublish(m_peers[index3]->subscribe_topicID[index2]);
									}
								}
								
								for (i=0;i<TOPIC_MAXNUM;i++)
								{
									if (m_topicmapping[i][0] != -1 )
									{
										count=0;
										for (j=1;j<PEER_MAXNUM_FOR_EACH_TOPIC;j++)
										{
											if (m_topicmapping[i][j] == index3)
											{
												topic = m_topicmapping[i][0];
												m_topicmapping[i][j] = -1;
												printf("peer %d removed in queue topic %d ...\n",index,m_topicmapping[i][0]);
											}
											else if (m_topicmapping[i][j] != -1)
												count++;
										}
										if (count==0)//该种topic无人订阅
										{
											printf("没有组件订阅该数据！\n");
											cdf->RemoteUnsubscribe(topic);
											m_topicmapping[i][0] = -1;
										}
									}		
								}
								delete m_peers[index3];
								m_peers[index3]=NULL;
							}
						}
					}
					
					delete m_peers[index];
					m_peers[index]=NULL;
				}
			}
		}
	}
	MUTEX_UNLOCK(peer_lock_);
	if(findinvalid)
		return 0;
	else
		return 1;

}
/************************************************************************/
/* 本组件需要发布数据主体时，通过通过CDataFlow调用
CDataChannel的该接口完成完成发布。
一个输入参数：需要在组播地址发送的报文；
报文格式： |主题|IP|端口号|
                                                                     */
/************************************************************************/
int CDataChannel::Publish(int topic){	
	csu->pack_UDP_Header_packet(MSG_PUBLISH,udpheader_,topic,
		multicast_host_addr,multicast_host_port,'0',0);
	if(multicast_peer->sendData(udpheader_,UDP_HEADER_LEN)){	
		return 1;
	}
	else{
		perror("publish through multicast error");
		return -1;
	}
}
/************************************************************************/
/* 本组件收到的数据的统计，用于上层组播
一个输入参数：需要在组播地址发送的报文；
报文格式： |主题|统计数|
                                                                     */
/************************************************************************/
int CDataChannel::publishFlowIn(TopicID topic,u_short reference)
{
	csu->pack_UDP_Header_packet(MSG_COMPSTATE,udpheader_,topic,
		multicast_host_addr,multicast_host_port,char(0),reference);
	if(multicast_peer->sendData(udpheader_,UDP_HEADER_LEN)){	
		return 1;
	}
	else{
		perror("publish through multicast error");
		return -1;
	}
}
/************************************************************************/
/* 本组件发送的数据的统计，用于上层组播
一个输入参数：需要在组播地址发送的报文；
报文格式： |主题|统计数|
                                                                     */
/************************************************************************/
int CDataChannel::publishFlowOut(TopicID topic,u_short reference){
	csu->pack_UDP_Header_packet(MSG_COMPSTATE,udpheader_,topic,
		multicast_host_addr,multicast_host_port,char(1),reference);
	if(multicast_peer->sendData(udpheader_,UDP_HEADER_LEN)){	
		return 1;
	}
	else{
		perror("publish through multicast error");
		return -1;
	}
}
/************************************************************************/
/* 本组件需要订阅指定主题的数据时，通过CDataFlow调用CDataChannel的该接口完成订阅。
两个输入参数：订阅主题、发布该主题数据组件的IP地址、端口号；
CDataChannel检查是否存在与指定IP地址、端口号对应的cpeer，如果找到，
则通过此cpeer发送订阅报文；如果没有找到，则调用CConnector建立连接，
并建立对应的CPeer，进而发送订阅报文。
Int Subscribe(TopicID topic, char* pAddr, int port)                                                                    */
/************************************************************************/
int CDataChannel::Subscribe(int topic, char* pAddr, u_short port){
	if (checksubscribe(topic,pAddr,port)>0)
	{
		printf("Subscribe msg send success \n");
		return 1;
	}
	printf("CDataChannel::Subscirbe(%d,%s,%d) \n",topic,pAddr,port);
	connector->Connect(0,pAddr,port);

	return checksubscribe(topic,pAddr,port);
}
/************************************************************************/
/* 本组件需要订阅指定主题的数据时，通过CDataFlow调用CDataChannel的该接口完成订阅。
两个输入参数：订阅主题、发布该主题数据组件的IP地址、端口号；
CDataChannel检查是否存在与指定IP地址、端口号对应的cpeer，如果找到，
则通过此cpeer发送订阅报文；如果没有找到，则调用CConnector建立连接，
并建立对应的CPeer，进而发送订阅报文。                                                                     */
/************************************************************************/
int CDataChannel::checksubscribe(int topic, char* pAddr, u_short port)
{
	MUTEX_LOCK(peer_lock_);
	u_short i;

	for (i=0;i<MAXPEERNUM;i++)
	{
		if ((m_peers[i]) && (m_peers[i]->compareAddrPort(pAddr,port)))
		{
			csu->pack_TCP_Header_packet(tcpheader_,MSG_SUBSCRIBE,topic,pAddr,port,0);
			if(m_peers[i]->sendData(tcpheader_,TCP_HEADER_LEN))
			{
				printf("send msg subscribe success...\n");
				m_peers[i]->insert_Subscribe_TopicID(topic);
				MUTEX_UNLOCK(peer_lock_);
				return 1;
			}
			else
			{
				MUTEX_UNLOCK(peer_lock_);
				return -1;
			}
		}
	}
	MUTEX_UNLOCK(peer_lock_);
	return -1;
}
/************************************************************************/
/* used to interupt select in run function                                                                     */
/************************************************************************/
int CDataChannel::Interrupt_select()
{
/*	char* interrupt_packet = */
	csu->pack_TCP_Header_packet(tcpheader_,MSG_ESTABLISH,0,NULL,0,0);
	int ret;
	if (interrupt_sender)
	{
		if((ret =interrupt_sender->sendData(tcpheader_,TCP_HEADER_LEN))==TCP_HEADER_LEN)
			printf("send interrput msg success...\n");
		else printf("send interrput msg failed...\n");
	}
	else printf("interrupt_sender didn't init ...\n");
	return ret;
}
void CDataChannel::Run()
{	
	int            i;
	fd_set         fdread;//维持一个SOCKET数组 最大FD_SETSIZE=64
	int            ret;

	int topic;
	u_short port;
	short type ;
	char* paddr;
	u_short len;
	int length;
	int tcpflag;
	char* multicast_packet= new char[UDP_HEADER_LEN];
	char* tcpheader= new char[TCP_HEADER_LEN];
	
	while (TRUE)
	{
		FD_ZERO(&fdread);
		FD_SET(interrupt_recver->s_,&fdread);
		FD_SET(multicast_peer->s_,&fdread);
		MUTEX_LOCK(peer_lock_);
		for (i = 0; i < MAXPEERNUM; i++)
		{
			if (m_peers[i]&& (m_peers[i]->s_) &&(m_peers[i]->valid_))
				FD_SET(m_peers[i]->s_, &fdread);	
		}
		MUTEX_UNLOCK(peer_lock_);
		// We only care read event
		ret = select(0, &fdread, NULL, NULL, NULL);
		//handle multicast data 
		if (FD_ISSET(multicast_peer->s_, &fdread))
		{
			ret =multicast_peer->receive(topic,multicast_packet,length); 
			if (ret==UDP_HEADER_LEN)
			{
				short multi_type = (unsigned char)multicast_packet[0];
				if(multi_type == MSG_PUBLISH)
				{
					topic = (((unsigned char)multicast_packet[1])<<24)|
						(((unsigned char)multicast_packet[2])<<16)|
						(((unsigned char)multicast_packet[3])<<8)|
						(((unsigned char)multicast_packet[4]));
					char paddr[128]={0};
					char tempchar[32]={0};
					for (i=0;i<4;i++)
					{
						sprintf(tempchar,"%d",(unsigned char)multicast_packet[8-i]);
						strcat(paddr,tempchar);
						strcat(paddr,".");
					}
					int index = strlen(paddr);
					paddr[index-1]='\0';
					port = (((unsigned char)multicast_packet[9])<<8)|
						(((unsigned char)multicast_packet[10]));
	//				printf("multicast data recved topic: %d  IP: %s  port: %d  \n",topic,paddr,port);
					//通知上层。。。
					domain_id tmpdomain = topic/DOMAIN_INTERVAL;
					if(tmpdomain == m_domainid)
						cdf->RemotePublish(topic,paddr,port);
				}
				else if (multi_type == MSG_COMPSTATE)
				{
					cdf->RemotePublishFlowInfo(multicast_packet);
				}
			}
			else if (ret<0)
			{
				printf("multicast recv failed...");
			}
			else 
			{
				printf("multicast recv a unparseable packet...");
			}
			
			
		}
		//handle common TCP link data 
		for (i = 0; i < MAXPEERNUM; i++)
		{
			if ((m_peers[i]) &&(m_peers[i]->s_)&&(FD_ISSET(m_peers[i]->s_, &fdread)))
			{
				// A read event happened on g_CliSocketArr
				tcpflag=0;//读取tcp头
				m_peers[i]->reading_ = 1;
				m_peers[i]->time_ = time(NULL);
				ret = m_peers[i]->receive(tcpflag,tcpheader,length);
				m_peers[i]->reading_ = 0;
				if (ret>=TCP_HEADER_LEN)
				{
					csu->unpack_TCP_Header_packet(type,tcpheader,topic,paddr,port,len);
					if (type == MSG_SUBSCRIBE)
					{
						insert_topic_peer_map(topic,i);
						cdf->RemoteSubscribe(topic);
						printf("MSG_SUBSCRIBE prased success...\n");
					}
					else if (type == MSG_UNSUBSCRIBE)
					{
						un_insert_topic_peer_map(topic,i);
						cdf->RemoteUnsubscribe(topic);
						printf("MSG_UNSUBSCRIBE prased success...\n");
					}
					else if (type == MSG_DATA_TCP)
					{
						char* newtcpdata=NULL;
						tcpflag = 1;
						length = len;
						m_peers[i]->reading_ = 1;
						m_peers[i]->time_ = time(NULL);
						ret = m_peers[i]->receive(tcpflag,newtcpdata,length);
						m_peers[i]->reading_ = 0;
						if(ret<=0)
						{
							printf("Client socket %d closed.\n", m_peers[i]->s_);
							for (int j=0;j<MAX_TOPIC_SIZE;j++)
							{
								if (m_peers[i]->subscribe_topicID[j]!=-1)
									cdf->RemoteUnpublish(m_peers[i]->subscribe_topicID[j]);
							}
							delPeerByIndex(i);
						}
						else
						{
							cdf->Send(topic,newtcpdata,length);

							printf("insert data from eth into dataflow ok\n");
							if (newtcpdata)
								free(newtcpdata);
						}

					}
					else if (type == MSG_UNPUBLISH)
					{
						cdf->RemoteUnpublish(topic);
					}
					else if(type == MSG_ESTABLISH)
					{
						printf("出了大问题了！！ %d , %d\n" , port , i);
					}
					else
					{
						printf("彻底崩溃了！！ %d , %d\n" , type , i);
					}
					printf("tcp data arrive \n");
					
				}
				//else if ((ret == SOCKET_ERROR && WSAGetLastError() == WSAECONNRESET))
				else if (ret == SOCKET_ERROR)
				{
					printf("Client socket %d closed.\n", m_peers[i]->s_);
					for (int j=0;j<MAX_TOPIC_SIZE;j++)
					{
						if (m_peers[i]->subscribe_topicID[j]!=-1)
							cdf->RemoteUnpublish(m_peers[i]->subscribe_topicID[j]);
					}
					delPeerByIndex(i);
					
				}
			}
		}
		//handle interrupt packet
		if (FD_ISSET(interrupt_recver->s_, &fdread))
		{
			tcpflag = 0;
			ret =interrupt_recver->receive(tcpflag,tcpheader,length); 
			if (ret==TCP_HEADER_LEN)
			{
				printf("interrupt data arrive \n");
			}
			continue;
		}
	}
	delete[] tcpheader;
	delete[] multicast_packet;
}

int CDataChannel::insert_topic_peer_map(int topic,int index)
{

	MUTEX_LOCK(peer_lock_);
	u_short i,j;
	for (i=0;i<TOPIC_MAXNUM;i++)
	{
		if (topic == m_topicmapping[i][0])
		{
			for (j=1;j<PEER_MAXNUM_FOR_EACH_TOPIC;j++)
			{
				if (m_topicmapping[i][j] == index)
				{
					printf("peer already in topicmaping\n");
					MUTEX_UNLOCK(peer_lock_);
					return 1;
				} 
			}
			for (j=1;j<PEER_MAXNUM_FOR_EACH_TOPIC;j++)//第一遍扫描peer不在映射中
			{
				if (m_topicmapping[i][j] == -1)
				{
					m_topicmapping[i][j] = index;
					printf("peer insert into topicmaping success...\n");
					MUTEX_UNLOCK(peer_lock_);
					return 1;	
				}
			}
			printf("topic already in topicmaping but no more space for new peer in the sending queue...\n");
			MUTEX_UNLOCK(peer_lock_);
			return -1;
		}		
	}
	for (i=0;i<TOPIC_MAXNUM;i++)//第一遍扫描 映射中没有topic
	{
		if (m_topicmapping[i][0] == -1)
		{
			m_topicmapping[i][0]=topic;
			m_topicmapping[i][1]=index;
			printf("create a new sending queue and peer insert into topicmaping success...\n");
			MUTEX_UNLOCK(peer_lock_);
			return 1;
		}
	}
	printf("no more space for a new sending queue......\n");
	MUTEX_UNLOCK(peer_lock_);
	return -1;
}
int CDataChannel::un_insert_topic_peer_map(int topic,int index){
	int i,j;
	for (i=0;i<TOPIC_MAXNUM;i++)
	{
		if (topic == m_topicmapping[i][0])
		{
			for (j=1;j<PEER_MAXNUM_FOR_EACH_TOPIC;j++)
			{
				if (m_topicmapping[i][j] == index)
				{
					m_topicmapping[i][j] = -1;
					printf("peer removed in topicmaping success...\n");
					return 1;
				}
			}
			return 1;
		}		
	}
	return 1;
}

int CDataChannel::UnPublish(TopicID topic){
	printf("CDataChannel::UnPublish\n");
	MUTEX_LOCK(peer_lock_);
	char* unpub_tcp_packet = new char[TCP_HEADER_LEN];
	csu->pack_TCP_Header_packet(unpub_tcp_packet,MSG_UNPUBLISH,topic,NULL,NULL,NULL);
	int i,j;
	for (i=0;i<TOPIC_MAXNUM;i++)
	{
		if (topic == m_topicmapping[i][0])
		{
			for (j=1;j<PEER_MAXNUM_FOR_EACH_TOPIC;j++)
			{
				if (m_topicmapping[i][j] != -1)
				{
					if (m_peers[m_topicmapping[i][j]])
					{
						m_peers[m_topicmapping[i][j]]->sendData(unpub_tcp_packet,TCP_HEADER_LEN);
					}
					printf("peer%d removed in topicmaping in queue topic: %d success...\n",m_topicmapping[i][j],topic);
					m_topicmapping[i][j] = -1;
				}
			}
			printf("topic:%d removed in topicmaping success...\n",topic);
			m_topicmapping[i][0]=-1;
		}		
	}
	MUTEX_UNLOCK(peer_lock_);
	return 1;
}

int CDataChannel::UnSubscribe(TopicID topic){
	printf("CDataChannel::UnSubscribe\n");
	char* unsub_tcp_packet = new char[TCP_HEADER_LEN];
	csu->pack_TCP_Header_packet(unsub_tcp_packet,MSG_UNSUBSCRIBE,topic,NULL,NULL,NULL);
	int i;
	for (i=0;i<MAXPEERNUM;i++)
	{
		if (m_peers[i]!=NULL && (m_peers[i]->find_Subscribe_TopicID(topic)==1))
		{
			m_peers[i]->sendData(unsub_tcp_packet,TCP_HEADER_LEN);
			printf("send UnPublish packet success....\n");
			m_peers[i]->delete_Subscribe_TopicID(topic);
		}
	}
	return 1;
}
