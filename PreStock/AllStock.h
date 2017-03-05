#ifndef ALLSTOCK_H
#define ALLSTOCK_H
#include "Configuration.h"
#include "SingleStock.h"


class AllStock
{
public:
    AllStock(unsigned int needToCalculateAmount);

    void calculateAllByDirectory(string path);

    void calculate_all_directory_date_constrained(string path,int &year,int &month,int &day);

    void calculateSingle(string path);

    void resultFusion();

    void recommandTop(int topnum);             //推荐最好的股票

    void recordAllPrediction();                //记录本次的所有预测


public:
    int stockNum;                              //股票数目
    std::vector<SingleStock> allstock;         //预测结果

    bool is_date_constrained;
    int year, month, day;
    unsigned int needToCalculateAmount;
};

#endif
