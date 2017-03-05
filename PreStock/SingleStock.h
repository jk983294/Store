#ifndef SINGLESTOCK_H
#define SINGLESTOCK_H

#include "Configuration.h"
#include "Matrix.h"


class SingleStock
{
    friend bool operator>(const SingleStock &a ,const SingleStock &b)  {  return a.result>b.result;}
    friend bool operator<(const SingleStock &a ,const SingleStock &b)  {  return a.result<b.result;}
    friend ostream& operator<<(ostream& os, const SingleStock &ss);
public:
    SingleStock();

    ~SingleStock(void);

    void freeResource();                                //释放资源，仅保存预测结果
    void load_dataset(string &path,unsigned int needToCalculateAmount);
    //有时间限制的读入数据集
    void load_dataset_date_constrained(string &path,int &year,int &month,int &day,unsigned int needToCalculateAmount);


    void calculate_all();                               //预测总入口
    //计算dataset中除了原始数据以外的数据
    void calculate_otherdata(unsigned int needToCalculateAmount);
    void calculate_quantity_activety();
    void calculate_price_activety();
    void calculate_time_statistics();
    void resultFusion();
    void save(string filename);
private:
    //dataset的columnIndex列数据的最后lastNum个数据用order阶多项式拟合，求拟合数据的第derivateTime阶导数
    double calculate_column_derative(int columnIndex, int begin, int end,  int order,int derivateTime);
    //dataset的columnIndex列数据的最后lastNum个数据的均值
    double calculate_column_mean(int columnIndex, int lastNum);
    //记录的时间解析为星期几，年，月做统计
    int pharse_time_statistics(string line);
public:
    string stockcode;
    bool toosmall;    //数据集太小，直接不用考虑
    //0      1     2     3    4     5     6     7
    //开盘    最高  最低  收盘  成交量 成交额 涨跌幅 振幅
    Matrix<double> data_set;
    unsigned int rowsN;

    bool is_time_statistics_on;
    Vector<int> time_statistics;                        //用来记录dataset每条记录的时间，可以是星期几，年，月做统计
    vector<string> date;                                //每条记录的时间

    //0     长期量能活跃程度
    //1     短期量能活跃程度
    //2     长期振幅活跃程度
    //3     短期振幅活跃程度
    Vector<double> results;                             //原始结果
    Vector<double> scaled_results;                      //规格化结果

    double result;  //最终结果

};

#endif
