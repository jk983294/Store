#include "Strategy.h"


double Asset::tax = 0.0075;
double Strategy::initalfund = 50000;

const double STOP_EARNING_LINE = 0.10;
const double STOP_LOSING_LINE = -0.10;
const double BIG_COLLAPSE_LINE = -0.05;
const double BIG_UP_LINE = 0.05;
const int STOP_LOSING_DAY = 10;

void Strategy::simulation(int begin_index){
    Matrix<double> &dataset = (*stock).data_set;
    begin_index = (begin_index < 0)? 0 : begin_index;
    int index = begin_index;
    int max_index = dataset.get_rows_number();

    ofstream actionfile("transaction.txt");
    if(!actionfile.is_open()){return;}
    actionfile << stock->stockcode << endl;
    actionfile << "totalasset   stackcount     buyprice      sellprice      newestprice     profit" << endl;


    while(index < max_index){
        //calculate something for today
        asset.newestprice = dataset[index][0];                  //open price of today
        isanomalous = (MathUtil::is_abs_bigger(dataset[index][6], 0.11))?true:false;
        if(isanomalous) {   asset.sell_all(dataset[index-1][3]);    decision = HOLDMONEY;
        actionfile << stock->date[index] <<" anomalous "<<endl;   index++;    continue;   }
        //make decision according to decision made by yesterday
        switch(strategydecision){
            case BUYSELL:{
                switch(decision){
                    case HOLDMONEY:{decision = BUY; break;}
                    case BUY:{
                        asset.buy_all(dataset[index][0]);       //buy according to  open price
                        decision = SELL;
                        actionfile << stock->date[index] <<" buy "<<asset<< endl;
                        break;
                    }
                    case SELL:{
                        asset.sell_all(dataset[index][0]);      //sell according to  open price
                        decision = BUY;
                        actionfile << stock->date[index] <<" sell "<<asset<< endl;
                        break;
                    }
                    default:{  break;}
                }
                break;
            }
            case OPTIMAL:{
                switch(decision){                               //根据前一天的决定
                    case HOLDMONEY:{break;
                    }
                    case WAIT:{
                        actionfile << stock->date[index] <<" WAIT "<<endl;
                        if((--wait)<= 0){   decision = HOLDMONEY;}
                        break;
                    }
                    case HOLDSTOCK:{break;}
                    case BUY:{
                        asset.buy_all(dataset[index][0]);       //buy according to  open price
                        asset.newestprice = dataset[index][3];  //close price of today
                        decision = HOLDSTOCK;
                        actionfile << stock->date[index] <<" buy "<<asset<< endl;
                        break;
                    }
                    case STOPLOSING:{
                        asset.sell_all(dataset[index][0]);      //sell according to  open price
                        asset.newestprice = dataset[index][3];  //close price of today
                        decision = WAIT; wait = 5;
                        actionfile << stock->date[index] <<" stop losing so sell all "<<asset<< endl;
                        break;
                    }
                    case SELL:{
                        asset.sell_all(dataset[index][0]);      //sell according to  open price
                        asset.newestprice = dataset[index][3];  //close price of today
                        decision = HOLDMONEY;
                        actionfile << stock->date[index] <<" sell "<<asset<< endl;
                        break;
                    }
                    default:{  break;}
                }
                break;
            }
        }
        //calculate something for today
        asset.newestprice = dataset[index][0];                  //open price of today
        isbigcollapse = (MathUtil::issmaller(dataset[index][6], BIG_COLLAPSE_LINE))?true:false;
        isbigup = (MathUtil::isbigger(dataset[index][6], BIG_UP_LINE))?true:false;
        make_tomorrow_decision(index);
        index++;
    }
    actionfile.close();
}
void Strategy::make_tomorrow_decision(int index){
    if(isbigcollapse){   make_WAIT_decision();   return;}
    if((decision == WAIT) && isbigup ){ decision = HOLDMONEY;}
//    if((decision == WAIT) && (wait == 0)){  decision = HOLDMONEY;   return;}
    Matrix<double> &dataset = (*stock).data_set;
    //(short[today]-long[today]) - (short[yesterday]-long[yesterday])   //短期与长期趋势变小
    if(decision == HOLDSTOCK  &&  ((dataset[index][8]-dataset[index-1][8]) <0) ){
        //短期走势一旦下降就卖
        decision = SELL;
    }
    else if(decision == HOLDMONEY  && ((dataset[index][8]-dataset[index][9]) -(dataset[index-1][8]-dataset[index-1][9])<0) ){
        decision = BUY;
    }
}
void Strategy::make_WAIT_decision(){
    if(decision == HOLDSTOCK) { decision = STOPLOSING;  return;}
    else{
        decision = WAIT;
        //big collapse wait collapse_wait days for recovery
        wait = STOP_LOSING_DAY;
    }
}
bool Asset::is_stop_losing(){
    if(stockcount > 0){
        return (MathUtil::issmaller(MathUtil::upratio(newestprice,buyprice), STOP_LOSING_LINE))?true:false;
    }else return false;
}
bool is_stop_earning();

double Asset::profit_ratio(){           // 资产收益
    profit = (total_asset() / Strategy::initalfund) -1;
    return profit;
}
void Asset::sell_all(double price){
    fund = fund + (1-tax)*stockcount*price;
    stockcount = 0; sellprice = price;
}
void Asset::buy_all(double price){      //按照price买入
    int hand = fund / ((1+tax)*price*100);
    stockcount = hand * 100;    newestprice = buyprice = price;
    fund = fund - stockcount * (1+tax) * price;
}
ostream& operator<<(ostream& os,  Asset &asset){
    os<<asset.total_asset()<<" "
        <<asset.stockcount<<" "<<asset.buyprice<<" "
        <<asset.sellprice<<" "<<asset.newestprice<<" "
        <<asset.profit_ratio()<<" ";
        return os;
}
