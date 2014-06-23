#ifndef CSENDER_H
#define CSENDER_H

/**************************************************	
类    名：Sender
功能说明：	使用该类完成原始数据的发送
**************************************************/
class CSender
{
public:
	CSender(){}

	~CSender(){}
	//发送数据
	virtual int Send(TopicID topic,char* buffer , int len)=0;
};

#endif