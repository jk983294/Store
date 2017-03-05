#ifndef MY_MATHUTIL_H
#define MY_MATHUTIL_H

#include <math.h>
#include <iostream>

const double epslong = 10e-7;

class MathUtil
{
public:
    static bool isbigger(double a,double b){ return a > b; }
    static bool issmaller(double a,double b){ return a < b;}
    static bool isbetween(double a,double from, double to){    return (a > from) && (a < to);}
    static bool is_abs_bigger(double a,double b){    return fabs(a) > b;}
    static double upratio(double now,double past){  return (fabs(past) < epslong) ? 0 : ((now/past) - 1);}
    //求正态分布概率
    static double normdist(double x, double mean, double standard_dev);
    //求正态分布概率密度函数
    static double normdistdensity(double x, double mean, double standard_dev);
    //标准正太分布值
    static double stardard_normdist(double x);
    //求斜率
    static double slope(double height,double length);
    //求通过两点直线，并求得x对应的y值，并且在x1,x2以外无定义的y = 0
    static double valueInLineThroughTwoPoints(double x,double x1,double y1,double x2,double y2);

    static double triangle_descend(double x,double center,double leftspan,double rightspan);
    //计算相应百分比后的价格
    static double valueByPrecentage(double currentvalue,double precentage);
    static void test();
};


#endif
