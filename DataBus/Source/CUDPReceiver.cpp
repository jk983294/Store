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
CUDPReceiver::CUDPReceiver(SOCKET s )
{
	s_ = s;	
	//buffer_ = 0;	
	buffer_len_ = 0;
	addrlen=sizeof(from);
	//Init_multicast();

	  
}
int CUDPReceiver::Init_multicast(){
	
	return 1;
}
/**************************************************	
�������ƣ�~Receiver
**************************************************/
CUDPReceiver::~CUDPReceiver()
{
	closesocket(s_);

	//buffer_ = 0;
	buffer_len_ = -1;

	Sleep(30);

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
**************************************************/
int CUDPReceiver::Receive(TopicID& topic, char*& data , int& length)
{
	int nbytes;
	    
	if ((nbytes=recvfrom(s_, buffer_, UDP_HEADER_LEN, 0, (struct sockaddr *) &from, (int *)&addrlen)) < 0)     
	{     
		printf("multicast error code :%d\n",GetLastError());
		return -1;    
	}  
	buffer_[nbytes] = 0;
	//printf("multicast data recv : '%s' from <%s>  in total %d bytes \n", buffer_, inet_ntoa(from.sin_addr),nbytes);
	//data= new char[UDP_HEADER_LEN];
	memcpy(data,buffer_,UDP_HEADER_LEN);
	return nbytes;
}
