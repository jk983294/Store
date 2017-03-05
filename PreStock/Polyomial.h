#ifndef _POLYOMIAL_H
#define _POLYOMIAL_H

#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include "Matrix.h"

using namespace std;


class PolyomialTerm
{
public:
    PolyomialTerm();

    PolyomialTerm(const PolyomialTerm& other);

    PolyomialTerm(double order,double coefficient);

    void set(double order,double coefficient);

    double result(double x);        //当前项在x处的值

    void derivate();                //求导
public:
    double order;                   //序
    double coefficient;             //系数
};

class Polyomial
{
public:
    Polyomial(double order);

    double result(double x);        //多项式在点x处的值

    void print();

    void polyfit();                 //多项式拟合

    void derivate();                //求导

    void yy_scale_precentage();     //y值根据最小y值进行缩放，变成相对涨幅后进行拟合

    void set_x_y(Vector<double> &x, Vector<double> &y);

    void set_order(int neworder);

    void saveAsCSVFormat(std::string &stockcode);

    static void test();
public:
    double order;                   //多项式最高次数
    Vector<PolyomialTerm> poly;
    Vector<double> xx;
    Vector<double> yy;
};


#endif
