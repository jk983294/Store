#ifndef STRATEGY_H
#define STRATEGY_H
#include "Configuration.h"
#include "SingleStock.h"

enum TomorrowDecision{
    HOLDMONEY,
    HOLDSTOCK,
    STOPEARNING,                    //止盈
    STOPLOSING,                     //止损
    WAIT,                           //等待wait过去
    BUY,
    SELL
};
enum StrategyDecision{
    BUYSELL,
    OPTIMAL
};
const StrategyDecision strategydecision = OPTIMAL;

class Asset{
    friend ostream& operator<<(ostream& os,  Asset &asset);
public:
    int stockcount;
    double buyprice, sellprice, newestprice, profit;
    double fund;
    static double tax;              //every transaction
public:
    Asset(){   stockcount=0; fund = sellprice = buyprice=newestprice = 0;};
    void set_fund(double fund){  this->fund = fund;};
    double total_asset(){   return fund + stockcount*newestprice;};     //现在的总价值
    double profit_ratio();          //资产收益
    void sell_all(double price);    //按照price卖出
    void buy_all(double price);     //按照price买入
    bool is_stop_losing();
    bool is_stop_earning();

};

class Strategy{
public:
    Asset asset;
    TomorrowDecision decision;
    vector<string> action_record;
    int wait;
    bool isanomalous,   isbigcollapse,  isbigup,  isstopearning,  isstoplosing;
    SingleStock *stock;
    static double initalfund;       // 启动资金
public:
    Strategy(SingleStock *ss){ asset.set_fund(initalfund); decision = HOLDMONEY; stock = ss;};
    void simulation(int begin_index);
    void make_WAIT_decision();
    void make_tomorrow_decision(int index);
};




#endif
