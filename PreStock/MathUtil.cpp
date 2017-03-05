#include "MathUtil.h"


const double STANDARD_NORM_MIDDLE = 0.39904;        //标准正太分布x=0时y轴的值
const double oor2pi = 1/(sqrt(double(2) * 3.1415926));

// x : 需要计算的概率坐标
// mean:正态分布的期望
// standard_dev： 标准差
double MathUtil::normdist(double x, double mean, double standard_dev)
{
    double res;
    x=(x - mean) / standard_dev;
    if (x == 0)
    {
        res=0.5;
    }
    else
    {
        double t = 1 / (double(1) + 0.2316419 * fabs(x));
        t *= oor2pi * exp(-0.5 * x * x)
            * (0.31938153   + t
            * (-0.356563782 + t
            * (1.781477937  + t
            * (-1.821255978 + t * 1.330274429))));
        if (x >= 0)
        {
            res = double(1) - t;
        }
        else
        {
            res = t;
        }
    }
    return res;
}
//求正态分布概率密度函数
double MathUtil::normdistdensity(double x, double mean, double standard_dev){
    double res;
    x=(x - mean) / standard_dev;
    res = (oor2pi * exp(-0.5 * x * x) )/standard_dev;
    return res;
}
//标准正太分布值
double MathUtil::stardard_normdist(double x){
    return normdist(x,0,1);
}
//求斜率
double MathUtil::slope(double height,double length){
    if (length == 0)
    {
        return 0;
    }
    else {
        return (height/length);
    }
}
//
/************************************************************************
 求通过两点直线，并求得x对应的y值，并且在x1,x2以外无定义的y = 0
                   |\
                  |  \
            _____|    \______
            y = k (x - x1) + y1
必须(x1,y1)在(x2,y2)左边
************************************************************************/
double MathUtil::valueInLineThroughTwoPoints(double x,double x1,double y1,double x2,double y2){
    double y = 0;
    if (x<x1 || x > x2)
    {
        return 0;
    }
    else {
        double k = (y2 - y1) / (x2 - x1);
        y = k * (x - x1) + y1;
    }
    return y;
}
/***********************************************************************
 三角形下降，并求得x对应的y值，并且在leftspan,rightspan以外无定义的y = 0
                    /|\
                  /  | \
            ____/____|  \______
***********************************************************************/
double MathUtil::triangle_descend(double x,double center,double leftspan,double rightspan){
    if ((x <= center - leftspan) || (x >= center + rightspan))
    {
        return 0;
    }
    else if (x > center)
    {
        return MathUtil::valueInLineThroughTwoPoints(x,center,1,center+rightspan,0);
    }
    else {
        return MathUtil::valueInLineThroughTwoPoints(x,center-rightspan,0,center,1);
    }
}
//计算相应百分比后的价格
double MathUtil::valueByPrecentage(double currentvalue, double precentage){
    return ( currentvalue * (1 + precentage * 0.01) );
}
void MathUtil::test(){
    double middle = MathUtil::normdist(0, 0 , 1);
    std::cout<<middle<<" "<<std::endl;
    std::cout<<MathUtil::normdistdensity(0.0108254, 0 , 1)<<" "<<std::endl;
    std::cout<<MathUtil::normdistdensity(0.0521127, 0 , 1)<<" "<<std::endl;
    std::cout<<MathUtil::normdistdensity(1, 0 , 1)<<" "<<std::endl;
//     std::cout<<MathUtil::valueInLineThroughTwoPoints(0.05 , 0.02 , 1 , 0.2 , 0)<<" "<<std::endl;
//     std::cout<<MathUtil::valueInLineThroughTwoPoints(0.1 , 0.02 , 1 , 0.2 , 0)<<" "<<std::endl;
//     std::cout<<MathUtil::valueInLineThroughTwoPoints(0.4 , 0.02 , 1 , 0.2 , 0)<<" "<<std::endl;
    std::cout<<MathUtil::triangle_descend(0.4 , 0 , 1 , 1 )<<" "<<std::endl;
    std::cout<<MathUtil::triangle_descend(-0.4 , 0 , 1 , 1 )<<" "<<std::endl;
}
