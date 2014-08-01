/***************************************************
文件名： Logger.cpp
说明：	日志类的源文件，定义该类函数
作者：   董丹
时间：	2013-08-1
版本：	复赛
***************************************************/


#include "Logger.h"

Logger::Logger(string path){
	MUTEX_INIT(mutex_logger);
	ofs.open(path.c_str(),ios::app);
	isgood = false;
	if (!ofs.good())	cout<<"日志模块初始化失败"<<endl;	
	else	{	log_time_ln("日志模块初始化成功...");	isgood = true;	}
}
Logger::~Logger(){
	if(isgood) {
		log_time_ln("日志模块关闭成功...");
		ofs.close();
	}
	MUTEX_DESTROY(mutex_logger);
}
void Logger::log_time_ln(string &line){
	MUTEX_LOCK(mutex_logger);
	ofs<<timeUtil::currentTime()<<" "<<line<<endl;
	MUTEX_UNLOCK(mutex_logger);
}
void Logger::log_time_ln(char *line){
	MUTEX_LOCK(mutex_logger);
	ofs<<timeUtil::currentTime()<<" "<<line<<endl;
	MUTEX_UNLOCK(mutex_logger);
}
void Logger::log_error_time_ln(string &line){
	MUTEX_LOCK(mutex_logger);
	if (isgood)	ofs<<timeUtil::currentTime()<<" "<<line<<" error code: "<<GetLastError()<<endl;
	MUTEX_UNLOCK(mutex_logger);
}
void Logger::log_error_time_ln(char *line){
	MUTEX_LOCK(mutex_logger);
	if (isgood)	ofs<<timeUtil::currentTime()<<" "<<line<<" error code: "<<GetLastError()<<endl;
	MUTEX_UNLOCK(mutex_logger);
}
void Logger::log_time(string &line){
	MUTEX_LOCK(mutex_logger);
	if (isgood)	ofs<<timeUtil::currentTime()<<" "<<line;
	MUTEX_UNLOCK(mutex_logger);
}
void Logger::log_time(char *line){
	MUTEX_LOCK(mutex_logger);
	if (isgood)	ofs<<timeUtil::currentTime()<<" "<<line;
	MUTEX_UNLOCK(mutex_logger);
}
void Logger::log_error_time(string &line){
	MUTEX_LOCK(mutex_logger);
	if (isgood)	ofs<<timeUtil::currentTime()<<" "<<line<<" error code: "<<GetLastError();
	MUTEX_UNLOCK(mutex_logger);
}
void Logger::log_error_time(char *line){
	MUTEX_LOCK(mutex_logger);
	if (isgood)	ofs<<timeUtil::currentTime()<<" "<<line<<" error code: "<<GetLastError();
	MUTEX_UNLOCK(mutex_logger);
}
void Logger::log(string &line){
	MUTEX_LOCK(mutex_logger);
	if (isgood)	ofs<<line;
}
void Logger::log(char *line){
	MUTEX_LOCK(mutex_logger);
	if (isgood)	ofs<<line;
	MUTEX_UNLOCK(mutex_logger);
}
void Logger::log_error(string &line){
	MUTEX_LOCK(mutex_logger);
	if (isgood)	ofs<<line<<" error code: "<<GetLastError();
	MUTEX_UNLOCK(mutex_logger);
}
void Logger::log_error(char *line){
	MUTEX_LOCK(mutex_logger);
	if (isgood)	ofs<<line<<" error code: "<<GetLastError();
	MUTEX_UNLOCK(mutex_logger);
}