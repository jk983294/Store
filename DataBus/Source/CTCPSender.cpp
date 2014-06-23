#include "../include/DataBus.h"

/**************************************************	
�������ƣ�Sender
���������s��SOCKET�����
*************************************************/
CTCPSender::CTCPSender(SOCKET s)
{
	s_ = s;
}

/**************************************************	
�������ƣ�~Sender
**************************************************/
CTCPSender::~CTCPSender()
{
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
int CTCPSender::Send(int topicID,char* buffer , int len)
{	
	int sendlen = 0;
	int tmplen = 0;
	char* tmpbuf = buffer;
	while(sendlen<len)
	{
		if((tmplen = send(s_ , tmpbuf , len-sendlen , 0)) == SOCKET_ERROR)
		{		
			int err = GetLastError();
			printf("Send Error : %d !\n" , err);		
			return -1;
		}
		sendlen+=tmplen;
		tmpbuf+=tmplen;
	}
	//printf("tcp sended %d bytes .\n",ret);	
	return len;	
}