#ifndef CRECEIVER_H
#define CRECEIVER_H

/**************************************************	
	��    ����Receiver
	����˵����
	  ���߳�ִ�й�����ѭ����ȡ��Ӧ�˿ڣ����ҽ����ݷ�װΪ
	  ��Ӧ�Ľṹ������յ�ԭʼ���ݣ���ԭʼ���ݴ洢�ڻ�����
**************************************************/
class CReceiver
{
public:
	CReceiver(){}

	~CReceiver(){}
	/*
	 *	���øýӿڵõ��洢��buffer_�е�ԭʼ���ݡ�ͨ���ź���sema_�Զ�д����
	 *	����ͬ��
	 */
virtual	int Receive(TopicID& topic, char*& data , int& length)=0;
};
#endif