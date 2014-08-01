/***************************************************
文件名： timeUtil.h
说明：	定义一个获取系统时间的类
作者：   董丹
时间：	2013-07-16
版本：	初赛
***************************************************/

#ifndef TIMEUTIL_H
#define TIMEUTIL_H

#include <time.h>
#include <string>
#include <sstream>
#include <iostream>
using namespace std;

class timeUtil
{
public:
	static std::string currentTime();	//获得当前时间 返回固定格式的时间string
};

#endif
