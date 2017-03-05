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
    static std::string currentTime();       //获得当前时间
    template<class T>   static string to_string(const T& t);
    static void test();

    void timeBegin();                       //计时开始
    void timeEnd();                         //计时结束
    void pharse_time(string line);          //2007-07-02
    //根据年月日得到星期几[0-6] 0代表星期天，1代表星期一
    int whichweekday();
    bool is_greater(string line);           //2007-07-02
    bool is_greater(int y1,int m1,int d1);  //this >= time1 ? true :false

    void printResult();
    void set(int y1,int m1,int d1){ year=y1;  month = m1;   day = d1;}
    timeUtil(int y1,int m1,int d1){ year=y1;  month = m1;   day = d1;}
    timeUtil(string line){ pharse_time(line);}
public:
    clock_t start;
    clock_t finish;
    int year,month,day;
};

#endif
