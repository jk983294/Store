/***************************************************
�ļ����� timeUtil.cpp
˵����	�����ȡϵͳʱ�����еĺ���
���ߣ�   ����
ʱ�䣺	2013-07-16
�汾��	����
***************************************************/

#include "timeUtil.h"

//��õ�ǰʱ�� ���ع̶���ʽ��ʱ��stringstream
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
