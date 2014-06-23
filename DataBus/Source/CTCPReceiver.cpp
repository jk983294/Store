#include "../include/DataBus.h"
/**************************************************	
�������ƣ�Receiver
���������
s��SOCKET�����
channelpeer��ͨ���������
����������ޡ�
����ֵ�� �ޡ�
����˵�����˺�����ɹ��캯��������
**************************************************/
CTCPReceiver::CTCPReceiver(SOCKET s )

{
	s_ = s;
	//MUTEX_INIT(recvLock_);	
	//tcp_header= new char[TCP_HEADER_LEN];
}

/**************************************************	
�������ƣ�~Receiver
**************************************************/
CTCPReceiver::~CTCPReceiver()
{
	closesocket(s_);

	Sleep(30);

//	MUTEX_UNLOCK(recvLock_);
	//MUTEX_DESTROY(recvLock_);
}
/**************************************************	
�������ƣ�receive
����������ޡ�
���������buffer���洢��ǰ��ԭʼ���ݵĻ��档
����ֵ�� ԭʼ���ݵĳ��ȡ�
����˵�����˺����õ��洢��buffer_�е�ԭʼ���ݡ�

  ��������·��صõ���ԭʼ���ݵĳ��ȣ���buffer��ָ��ԭʼ���ݵĻ���
  ����ڽ��յĹ����г��������쳣�򷵻�-1��buffer��0
  buffer�����ݴ�����Receiver::run()�� ͨ��Receiver::receive() ֮��ĵ��ô��ݸ�InfoList::insertData�е�InfoNode::setRawData(rawdata , len), ����һ�ν���ԭʼ����ʱ, InfoNode::setRawData(rawdata , len)�Ὣ���ε�buffer�������ͷ�

ѭ����ȡ����˿ڣ����õ������ݽ��д洢Ȼ��ͨ���ź����ȴ���ȡ
TopicID& flag: 0 read tcp header  1 :read tcp data
**************************************************/
int CTCPReceiver::Receive(TopicID& flag, char*& data , int& length)
{
	int ret =0;	
	if (flag == 0)
	{
		int recvlen = 0;
		int tmplen = 0;
		char* tmpbuf = tcp_header;
		while(recvlen<TCP_HEADER_LEN)
		{
			if((tmplen = recv(s_ , tmpbuf , TCP_HEADER_LEN-recvlen , 0)) == SOCKET_ERROR)
			{
				int err = GetLastError();
				printf("Recv Error : %d !\n" , err);
				return -1;
			}
			recvlen+=tmplen;
			tmpbuf+=tmplen;
		}
		memcpy(data , tcp_header , TCP_HEADER_LEN);
		return TCP_HEADER_LEN;
	}
	else if (flag == 1)
	{
		data = (char*)malloc(length);
		char* tmpbuf = data;
		memset(data , 0 , length);
		int recvlen = 0;
		int tmplen = 0;
		while(recvlen<length)
		{
			if((tmplen = recv(s_ , tmpbuf , length - recvlen , 0)) == SOCKET_ERROR)
			{
				int err = GetLastError();
				printf("Recv Error : %d !\n" , err);
				return -1;
			}
			recvlen+=tmplen;
			tmpbuf+=tmplen;
		}
		return length;
	}
	return -1;
		
}
