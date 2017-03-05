#include "Polyomial.h"


void Polyomial::test(){
    Vector<double> x{0, 1, 2, 3, 4, 5};
    Vector<double> y{17.0600, 15.4200, 15.3100, 15.2900, 13.9700, 14.1900};

    Polyomial poly(1);
    poly.set_x_y(x,y);
    poly.yy_scale_precentage();

    poly.polyfit();
    cout<<poly.result(6)<<endl;
    poly.derivate();
    cout<<poly.result(6)<<endl;
}

PolyomialTerm::PolyomialTerm(){
    order = 0;
    coefficient = 0;
}
PolyomialTerm::PolyomialTerm(const PolyomialTerm& other){
    this->order = other.order;
    this->coefficient = other.coefficient;
}
PolyomialTerm::PolyomialTerm(double order,double coefficient){
    this->order = order;
    this->coefficient = coefficient;
}
void PolyomialTerm::set(double order,double coefficient){
    this->order = order;
    this->coefficient = coefficient;
}
double PolyomialTerm::result(double x){
    return coefficient * pow(x,order);
}
//求导
void PolyomialTerm::derivate(){
    if (order == 0)
    {
        coefficient = 0;
        order = 0;
    }
    else{
        coefficient *= order;
        order--;
    }
}

Polyomial::Polyomial(double order){
    this->order = order;
    int count = order + 1;
    poly.resize(count);
    for (int i = 0; i <count; i++)
    {
        poly[i].set(i,0);
    }
}
void Polyomial::set_order(int neworder){
    this->order = neworder;
    int count = neworder + 1;
    poly.resize(count);
    for (int i = 0; i <count; i++)
    {
        poly[i].set(i,0);
    }
}
//多项式在点x处的值
double Polyomial::result(double x){
    double result = 0;
    for (unsigned int i = 0; i < poly.size(); i++)
    {
        result += poly[i].result(x);
    }
    return result;
}
void Polyomial::print(){
    for (unsigned int i = 0; i < poly.size(); i++)
    {
        std::cout<<poly[i].coefficient<<"\t";
    }
    std::cout<<std::endl;
    for (unsigned int i = 0; i < poly.size(); i++)
    {
        std::cout<<poly[i].order<<"\t";
    }
    std::cout<<std::endl;
}
//多项式拟合 A= (XT X)-1 (XT Y)   X为范德蒙矩阵
//具体原理见有道云笔记: 最小二乘法多项式曲线拟合原理与实现
void Polyomial::polyfit(){
    Matrix<double> X(xx.size(), poly.size());

    for (unsigned int i = 0; i < xx.size(); i++)
    {
        for (unsigned int j = 0; j < poly.size(); j++)
        {
            X[i][j] = pow(xx[i],poly[j].order);
        }
    }

    Matrix<double> X_T = X.calculate_transpose();       //X的转置
    Vector<double> A = (X_T.dot(X).calculate_inverse()).dot(X_T.dot(yy));
    for (unsigned int i = 0; i < A.size();i++)
    {
        poly[i].coefficient = A[i];
    }
}
//求导
void Polyomial::derivate(){
    for (unsigned int i = 0; i< poly.size();i++)
    {
        poly[i].derivate();
    }
}
// y值根据最小y值进行缩放，变成相对涨幅后进行拟合
void Polyomial::yy_scale_precentage(){
//    double minimum = yy.calculate_minimum();
//    if (minimum > 0){   yy /= minimum;}
    yy.scale_self_minimum_maximum();
}
void Polyomial::set_x_y(Vector<double> &x, Vector<double> &y){
    xx = x;
    yy = y;
}
void Polyomial::saveAsCSVFormat(std::string &stockcode){
    std::string filepath = "./data/PolyResult" + stockcode +".txt";

    std::ofstream fout(filepath.c_str());

    for (unsigned int i = 0;i < xx.size();i++)
    {
        fout<<xx[i]<<"\t"<<yy[i]<<"\n";
    }

    for (unsigned int i = 0; i < poly.size(); i++)
    {
        fout<<poly[i].coefficient<<"\t";
    }
    fout<<"\n";
    for (unsigned int i = 0; i < poly.size(); i++)
    {
        fout<<poly[i].order<<"\t";
    }
    fout<<"\n";

    fout.flush();
    fout.close();
}
