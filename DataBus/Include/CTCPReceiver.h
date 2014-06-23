#ifndef CTCPRECEIVER_H
#define CTCPRECEIVER_H

/**************************************************	
	��    ����Receiver
	����˵����
	  ���߳�ִ�й�����ѭ����ȡ��Ӧ�˿ڣ����ҽ����ݷ�װΪ
	  ��Ӧ�Ľṹ������յ�ԭʼ���ݣ���ԭʼ���ݴ洢�ڻ�����
**************************************************/
class CTCPReceiver:public CReceiver
{
public:
	CTCPReceiver(SOCKET s );


	~CTCPReceiver();
	/*
	 *	���øýӿڵõ��洢��buffer_�е�ԭʼ���ݡ�ͨ���ź���sema_�Զ�д����
	 *	����ͬ��
	 */
	int Receive(TopicID& topic, char*& data , int& length);

	/*
	 *	ѭ����ȡ����˿ڣ�����Ϣ�������һ��,��������������Ϣ��ʱ����Ҫ
	 *	ִ��clearDida�������dida_��������ԭʼ���ݵ�ʱ����Ҫ��ԭʼ���ݴ�
	 *	����buffer_�У�Ȼ���޸��ź�����
	 */
	
private:

	//SOCKET���
	SOCKET s_;

	//�洢��ǰ��ԭʼ���ݵĻ���
	char tcp_header[TCP_HEADER_LEN];
};
#endif