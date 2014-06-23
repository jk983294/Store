/***************************************************
文件名： timeUtil.cpp
说明：	定义获取系统时间类中的函数
作者：   董丹
时间：	2013-07-16
版本：	初赛
***************************************************/

#include "timeUtil.h"

//获得当前时间 返回固定格式的时间stringstream
std::string timeUtil::currentTime(){
	time_t now = time(NULL);
	time(&now);
	struct tm* tmTmp = localtime(&now);

	std::stringstream sstr;
	sstr<< (tmTmp->tm_year + 1900)<<"-"<<(tmTmp->tm_mon + 1)
		<<"-"<<(tmTmp->tm_mday)<<" "<<(tmTmp->tm_hour)<<":"
		<<(tmTmp->tm_min)<<":"<<(tmTmp->tm_sec);

	return sstr.str();
}
