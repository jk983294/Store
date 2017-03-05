#include "timeUtil.h"


bool timeUtil::is_greater(int y1,int m1,int d1){    //this >= time1 ? true :false
    if(year == y1){
        if(month == m1){
            if(day == d1) return true;
            else return day > d1;
        }
        else return month > m1;
    }
    else return year > y1;
}
bool timeUtil::is_greater(string line){
    line[7] = line[4] = ' ';
    istringstream is(line);
    int y1,m1,d1;
    is >> y1 >> m1 >> d1;
    return is_greater(y1,m1,d1);
}
void timeUtil::pharse_time(string line){            //2007-07-02
    line[7] = line[4] = ' ';
    istringstream is(line);
    is>>year>>month>>day;
}
template<class T>
string timeUtil::to_string(const T& t){
    ostringstream oss;
    oss<<t;
    return oss.str();
}
int timeUtil::whichweekday()                        //根据年月日得到星期几[0-6]
{
    struct tm t;
    time_t t_of_day;
    t.tm_year=year-1900;
    t.tm_mon=month - 1;                             //struct tm中tm_mon为[0-11]
    t.tm_mday=day;
    t.tm_hour=12;
    t.tm_min=0;
    t.tm_sec=0;
    t.tm_isdst=0;
    t_of_day=mktime(&t);
    return t.tm_wday;
}

//获得当前时间
std::string timeUtil::currentTime(){
    time_t now;
    struct tm* tmTmp;
    now = time(NULL);
    time(&now);
    tmTmp = localtime(&now);

    std::string timestring;
    std::stringstream sstr;
    sstr<< (tmTmp->tm_year + 1900)<<(tmTmp->tm_mon + 1)
        <<(tmTmp->tm_mday)<<(tmTmp->tm_hour)
        <<(tmTmp->tm_min)<<(tmTmp->tm_sec);
    sstr>>timestring;
    return timestring;
}

void timeUtil::test(){
    std::cout<<currentTime();
}
//计时开始
void timeUtil::timeBegin(){
    start = clock();
}
//计时结束
void timeUtil::timeEnd(){
    finish = clock();
}

void timeUtil::printResult(){
    int total = ((finish - start) / CLOCKS_PER_SEC);
    int seconds = total % 60;
    int minutes = (total / 60) % 60;
    int hours =  (total / (60 * 60)) % 60;

    std::cout<<hours<<" hours "<<minutes<<" minutes "<<seconds<<" seconds "<<std::endl;
}
