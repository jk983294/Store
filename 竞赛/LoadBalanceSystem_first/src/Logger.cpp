/***************************************************
�ļ����� Logger.cpp
˵����	��־���Դ�ļ���������ຯ��
���ߣ�   ����
ʱ�䣺	2013-07-16
�汾��	����
***************************************************/


#include "Logger.h"

Logger::Logger(string path){
	ofs.open(path.c_str(),ios::app);
	isgood = false;
	if (!ofs.good())	cout<<"��־ģ���ʼ��ʧ��"<<endl;	
	else	{	log_time_ln("��־ģ���ʼ���ɹ�...");	isgood = true;	}
}
Logger::~Logger(){
	if(isgood) {
		log_time_ln("��־ģ��رճɹ�...");
		ofs.close();
	}
}
void Logger::log_time_ln(string &line){
	ofs<<timeUtil::currentTime()<<" "<<line<<endl;
}
void Logger::log_time_ln(char *line){
	ofs<<timeUtil::currentTime()<<" "<<line<<endl;
}
void Logger::log_error_time_ln(string &line){
	if (isgood)	ofs<<timeUtil::currentTime()<<" "<<line<<" error code: "<<GetLastError()<<endl;
}
void Logger::log_error_time_ln(char *line){
	if (isgood)	ofs<<timeUtil::currentTime()<<" "<<line<<" error code: "<<GetLastError()<<endl;
}
void Logger::log_time(string &line){
	if (isgood)	ofs<<timeUtil::currentTime()<<" "<<line;
}
void Logger::log_time(char *line){
	if (isgood)	ofs<<timeUtil::currentTime()<<" "<<line;
}
void Logger::log_error_time(string &line){
	if (isgood)	ofs<<timeUtil::currentTime()<<" "<<line<<" error code: "<<GetLastError();
}
void Logger::log_error_time(char *line){
	if (isgood)	ofs<<timeUtil::currentTime()<<" "<<line<<" error code: "<<GetLastError();
}
void Logger::log(string &line){
	if (isgood)	ofs<<line;
}
void Logger::log(char *line){
	if (isgood)	ofs<<line;
}
void Logger::log_error(string &line){
	if (isgood)	ofs<<line<<" error code: "<<GetLastError();
}
void Logger::log_error(char *line){
	if (isgood)	ofs<<line<<" error code: "<<GetLastError();
}