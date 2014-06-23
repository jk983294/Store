/***************************************************
�ļ����� Logger.h
˵����	������־���ͷ�ļ�
���ߣ�   ����
ʱ�䣺	2013-07-16
�汾��	����
***************************************************/



#ifndef LOGGER_H
#define LOGGER_H


#include "Configure.h"
using namespace std;

class Logger{
public:
	ofstream ofs;
	bool isgood;
public:
	Logger(string path);
	~Logger();
	void log_time_ln(string &line);			//��¼ǰ���ʱ���������ӻ���
	void log_time_ln(char *line);				//��¼ǰ���ʱ���������ӻ���
	void log_error_time_ln(string &line);	//��¼ǰ���ʱ������Զ��Ӵ�����룬����ӻ���
	void log_error_time_ln(char *line);		//��¼ǰ���ʱ������Զ��Ӵ�����룬����ӻ���
	void log_time(string &line);				//��¼ǰ���ʱ��������治����
	void log_time(char *line);					//��¼ǰ���ʱ��������治����
	void log_error_time(string &line);		//��¼ǰ���ʱ������Զ��Ӵ�����룬���治����
	void log_error_time(char *line);			//��¼ǰ���ʱ������Զ��Ӵ�����룬���治����
	void log(string &line);						//��¼ǰ�治��ʱ��������治����
	void log(char *line);							//��¼ǰ�治��ʱ��������治����
	void log_error(string &line);				//��¼ǰ�治��ʱ������Զ��Ӵ�����룬���治����
	void log_error(char *line);					//��¼ǰ�治��ʱ������Զ��Ӵ�����룬���治����
};


#endif