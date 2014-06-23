#ifndef APPENCAPSULATE_H
#define APPENCAPSULATE_H

#ifdef DLL_EXPORTS
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

#define TopicID int
#define domain_id int

const int MAXOPERATIONUM = 8;
const int MAXINFOHANDLERNUM = 8;

const int DOMAIN_INTERVAL = 10000;

const int MAX_TOPIC_SEGMENTS = 4;
const int MAX_TOPIC_SEGLENG = 16;
const int MAX_CDATA_LENGTH = 8;

/**************** Fake Classes *****************/

class CDataFlow;
class CSendingArray;
class CReceivingArray;

/***********************************************/

class DLL_API CTopic;
class DLL_API CAny;
class DLL_API CData;
class DLL_API DomainParticipant;
class DLL_API DomainParticipantFactory;
class DLL_API CThread;
class DLL_API CPublisher;
class DLL_API CSubscriber;
class DLL_API CDataWriter;
class DLL_API CDataReader;
class DLL_API CDataDispatchor;
class DLL_API CDataHandler;
//class DLL_API CDataProxy;


#include "Topic.h"
#include "Any.h"
#include "Data.h"
#include "Thread.h"
#include "Publisher.h"
#include "Subscriber.h"
#include "DomainParticipant.h"
#include "DomainParticipantFactory.h"
#include "DataWriter.h"
#include "DataReader.h"
#include "DataDispatchor.h"
#include "DataHandler.h"
//#include "DataProxy.h"
#include "FakeClasses.h"

DLL_API int Initialize(domain_id domain);
DLL_API CTopic CreateTopic();
DLL_API CData CreateData(CTopic topic);
DLL_API int Publish(CTopic topic);
DLL_API int UnPublish(CTopic topic);
DLL_API int UnPublish(TopicID topic);
DLL_API int Subscribe(CTopic topic);
DLL_API int UnSubscribe(CTopic topic);
DLL_API int UnSubscribe(TopicID topic);
DLL_API int Send(CData data);

#define IMPORT_DATABUS extern DLL_API CDataDispatchor* g_datadispatcher_ptr_; \
extern DLL_API CDataFlow* g_dataflow_ptr_; \
extern DLL_API CSendingArray* g_sendingarray_ptr_;\
extern DLL_API CReceivingArray* g_receivingarray_ptr_;\
extern DLL_API DomainParticipantFactory* g_domainparticipantfactory_ptr_;\
extern DLL_API DomainParticipant* g_domainparticipant_ptr_;\
extern DLL_API CPublisher* g_publisher_ptr_;\
extern DLL_API CSubscriber* g_subscriber_ptr_;

#endif