#ifndef CSENDER_H
#define CSENDER_H

/**************************************************	
��    ����Sender
����˵����	ʹ�ø������ԭʼ���ݵķ���
**************************************************/
class CSender
{
public:
	CSender(){}

	~CSender(){}
	//��������
	virtual int Send(TopicID topic,char* buffer , int len)=0;
};

#endif