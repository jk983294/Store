/***************************************************
�ļ����� timeUtil.h
˵����	����һ����ȡϵͳʱ�����
���ߣ�   ����
ʱ�䣺	2013-08-1
�汾��	����
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
	static string currentTime();	//��õ�ǰʱ�� ���ع̶���ʽ��ʱ��string
	static string getSecondstring();            //��õ���ǰʱ�������,����Ϊstring
	static long getSecondlong();        //��õ���ǰʱ�������,������
};

#endif
