#include "../include/DataBus.h"

/**************************************************	
�������ƣ�Sender
���������s��SOCKET�����
*************************************************/

CUDPSender::CUDPSender(SOCKET s)
{
	s_ = s;
	/* set up destination address */    
    memset(&dest_addr,0,sizeof(dest_addr));    
    dest_addr.sin_family=AF_INET;    
    dest_addr.sin_addr.s_addr=inet_addr(MULTICAST_GROUP);    
    dest_addr.sin_port=htons(MULTICAST_PORT);  
}

/**************************************************	
�������ƣ�~Sender
**************************************************/
CUDPSender::~CUDPSender()
{
	closesocket(s_);
}

/**************************************************	
�������ƣ�sendData
���������
buffer�����ͻ�������
len���������ݳ��ȡ�
����������ޡ�
����ֵ�� ���ͳɹ����ط��ͳ��ȣ����򷵻�-1��
����˵�����˺�����Զ˷������ݡ�
**************************************************/
int CUDPSender::Send(int topicID,char* buffer , int len)
{	
	//����ͷ��Ϣ(DELETED)
	
	//���벢����ԭʼ����
	int send_success_length=0;
	if ((send_success_length =sendto(s_,buffer, len, 0, 
		(struct sockaddr *) &dest_addr, sizeof(dest_addr))) < 0)     
	{    
		perror("sendto"); 
		return -1;   
	}  
	//printf("sended %d bytes\n",send_success_length);
	return send_success_length;	
}