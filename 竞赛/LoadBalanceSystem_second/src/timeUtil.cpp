/***************************************************
文件名： timeUtil.cpp
说明：	定义获取系统时间类中的函数
作者：   董丹
时间：	2013-08-1
版本：	复赛
***************************************************/

#include "timeUtil.h"

//获得当前时间 格式为(年月日 时分秒) 返回固定格式的时间string 
string timeUtil::currentTime(){
	time_t now = time(NULL);
	time(&now);
	struct tm* tmTmp = localtime(&now);

	std::stringstream sstr;
	sstr<< (tmTmp->tm_year + 1900)<<"-"<<(tmTmp->tm_mon + 1)
		<<"-"<<(tmTmp->tm_mday)<<" "<<(tmTmp->tm_hour)<<":"
		<<(tmTmp->tm_min)<<":"<<(tmTmp->tm_sec);

	return sstr.str();
}
//获得到当前时间的秒数，类型为string
string timeUtil::getSecondstring()
{
	time_t now = time(NULL);
	std::stringstream sstr;
	sstr<<now;
	return sstr.str();

}
//获得到当前时间的秒数，类型为长整型
long timeUtil::getSecondlong(){
	unsigned long now = time(NULL);
	return now;
}
