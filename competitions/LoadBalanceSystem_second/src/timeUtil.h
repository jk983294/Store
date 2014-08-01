/***************************************************
文件名： timeUtil.h
说明：	定义一个获取系统时间的类
作者：   董丹
时间：	2013-08-1
版本：	复赛
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
	static string currentTime();	//获得当前时间 返回固定格式的时间string
	static string getSecondstring();            //获得到当前时间的秒数,类型为string
	static long getSecondlong();        //获得到当前时间的秒数,长整形
};

#endif
