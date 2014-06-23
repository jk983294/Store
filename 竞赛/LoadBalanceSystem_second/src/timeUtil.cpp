/***************************************************
�ļ����� timeUtil.cpp
˵����	�����ȡϵͳʱ�����еĺ���
���ߣ�   ����
ʱ�䣺	2013-08-1
�汾��	����
***************************************************/

#include "timeUtil.h"

//��õ�ǰʱ�� ��ʽΪ(������ ʱ����) ���ع̶���ʽ��ʱ��string 
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
//��õ���ǰʱ�������������Ϊstring
string timeUtil::getSecondstring()
{
	time_t now = time(NULL);
	std::stringstream sstr;
	sstr<<now;
	return sstr.str();

}
//��õ���ǰʱ�������������Ϊ������
long timeUtil::getSecondlong(){
	unsigned long now = time(NULL);
	return now;
}
