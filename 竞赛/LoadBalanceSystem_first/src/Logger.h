/***************************************************
文件名： Logger.h
说明：	定义日志类的头文件
作者：   董丹
时间：	2013-07-16
版本：	初赛
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
	void log_time_ln(string &line);			//记录前面加时间戳，后面加换行
	void log_time_ln(char *line);				//记录前面加时间戳，后面加换行
	void log_error_time_ln(string &line);	//记录前面加时间戳，自动加错误代码，后面加换行
	void log_error_time_ln(char *line);		//记录前面加时间戳，自动加错误代码，后面加换行
	void log_time(string &line);				//记录前面加时间戳，后面不换行
	void log_time(char *line);					//记录前面加时间戳，后面不换行
	void log_error_time(string &line);		//记录前面加时间戳，自动加错误代码，后面不换行
	void log_error_time(char *line);			//记录前面加时间戳，自动加错误代码，后面不换行
	void log(string &line);						//记录前面不加时间戳，后面不换行
	void log(char *line);							//记录前面不加时间戳，后面不换行
	void log_error(string &line);				//记录前面不加时间戳，自动加错误代码，后面不换行
	void log_error(char *line);					//记录前面不加时间戳，自动加错误代码，后面不换行
};


#endif