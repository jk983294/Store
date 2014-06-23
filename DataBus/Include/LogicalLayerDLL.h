/*
DLL”√¿‡
*/
#ifndef LOGICALLAYERDLL_H
#define LOGICALLAYERDLL_H

#include "util_macro.h"
#include "CDataArray.h"
#include "CDataFlow.h"
#include "CDataChannel.h"
#include "CNetworkOperator.h"

class DECLSPEC DllCall{
public:
	
	void Initialize();

	int Publish(TopicID);
	int Subscribe(TopicID);
	
	int Send(TopicID topic, char* data , int length);
	int Receive(TopicID& topic, char*& data , int& length);

	int Unpublish(TopicID);
	int Unsubscribe(TopicID);
private:
	CDataFlow* cdf;
	CDataArray* csa;
	CDataArray* cra;
	
	CNetworkOperator* cno;

};


#endif