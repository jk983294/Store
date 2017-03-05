#include "SingleStock.h"
#include "Polyomial.h"

/**
弱市中找反转
依据：
曾经量能放大过，现在调整量能萎缩 max（长期量能活跃程度 - 短期量能活跃程度 + 长期价格活跃程度 - 短期价格活跃程度）

强市中找强势
依据
量能逐渐放大  max （长期量能活跃程度 + 短期量能活跃程度 + 长期价格活跃程度 + 短期价格活跃程度）
*/

void SingleStock::resultFusion(){
    if(predictobject == WeakReverse){
        result = scaled_results[2] - scaled_results[3] ;
    }
    else if(predictobject == Strong){
        result = scaled_results[0] + scaled_results[1];
    }
}
void SingleStock::calculate_all(){
    if(!toosmall){
        calculate_quantity_activety();
        calculate_price_activety();
    }
}
void SingleStock::calculate_quantity_activety(){
    int end = rowsN-1,  begin = rowsN-20;
    //取第4列成交量的最后20个数据，1次直线拟合求斜率得到长期量能活跃程度
    results[0] = calculate_column_derative(4,begin,end,1,1);
    //取第4列成交量的最后20个数据，2次曲线拟合求斜率得到短期量能活跃程度
    results[1] = calculate_column_derative(4,begin,end,2,1);
}
void SingleStock::calculate_price_activety(){
    //取第7列振幅的最后15个数据的均值作为长期振幅活跃程度
    results[2] = calculate_column_mean(7,15);
    //取第7列振幅的最后5个数据的均值作为短期振幅活跃程度
    results[3] = calculate_column_mean(7,5);
}

void SingleStock::calculate_time_statistics(){
    if(!is_time_statistics_on)   return;
}
//dataset的columnIndex列数据的最后lastNum个数据用order阶多项式拟合，求拟合数据的第derivateTime导数
double SingleStock::calculate_column_derative(int columnIndex, int begin, int end,  int order,int derivateTime){
    Vector<double> x(0,1,end-begin);
    Vector<double> y = data_set.arrange_column_range(columnIndex,begin,end);
    Polyomial polyo(order);
    polyo.set_x_y(x,y);
    polyo.yy_scale_precentage();
    polyo.polyfit();
    for(int i = 0; i< derivateTime; i++){polyo.derivate();}
    return polyo.result(end-begin);
}
int SingleStock::pharse_time_statistics(string line)        //记录的时间解析为星期几，年，月做统计
{
    timeUtil tu(line);
    return tu.whichweekday();
}

//dataset的columnIndex列数据的最后lastNum个数据的均值
double SingleStock::calculate_column_mean(int columnIndex, int lastNum){
    return data_set.arrange_column_last(columnIndex,lastNum).calculate_mean();
}
//计算dataset中除了原始数据以外的数据
void SingleStock::calculate_otherdata(unsigned int needToCalculateAmount){
    //数据集中只需要计算的数目,其余不需要算
    unsigned int calculateMore = 15;                        //多算calculateMore个，方便用到前面的数据计算
    unsigned int needToCalculate = needToCalculateAmount + calculateMore;
    int n_short = 6;                                        //短n日一阶趋势
    int n_long = 10;                                        //长n日一阶趋势
    for (unsigned int i = rowsN - needToCalculate; i < rowsN; i++) {
        //计算今日涨跌幅
        if((i >= 1)   && (data_set[i-1][3] > 0)){ data_set[i][6] = (data_set[i][3] - data_set[i-1][3])/(data_set[i-1][3]);}
        else {  data_set[i][6] = 0; }
        //计算今日振幅    (今日最高-最低)/最低
        if((i >= 0)   && (data_set[i][2] > 0)){ data_set[i][7] = (data_set[i][1] - data_set[i][2])/(data_set[i][2]);}
        else {  data_set[i][7] = 0; }
        //计算n日一阶趋势
        data_set[i][8] = calculate_column_derative(3, i-n_short+1, i, 1, 1 );
        data_set[i][9] = calculate_column_derative(3, i-n_long+1, i, 1, 1 );
    }
}

//读入数据集，最后一行是预测数据
void SingleStock::load_dataset(string &path,unsigned int needToCalculateAmount){
    ifstream file(path.c_str());
    if(!file.is_open())
    {
        toosmall = true;
        cout << "Cannot open stock file.\n"<<endl;
        return;
    }
    // Set matrix sizes
    string line;
    getline(file, line);
    getline(file, line);                                    //去掉前两行
    rowsN = 0;
    while(file.good()){
        getline(file, line);
        if(line.size() > 20) {   rowsN++;}
    }
    if (rowsN > smallcretia){   toosmall = false;}

    // Clear file
    if(!toosmall){
        file.clear();
        file.seekg(0, ios::beg);
        getline(file, line);
        getline(file, line);                                //去掉前两行
        data_set.set(rowsN, attributes_count);
        date.resize(rowsN);

        if(is_time_statistics_on){  time_statistics.resize(rowsN);  }

        for(unsigned int i = 0; i < rowsN; i++)
        {
            //去掉原始数据中的时间
            for(unsigned int j = 0; j < raw_attributes_count+1; j++)
            {
                if(j > 0)
                    file >> data_set.data[i][j-1];
                else{
                    file>>line;     date[i] = line;
                    if(is_time_statistics_on){
                        time_statistics[i] = pharse_time_statistics(line);
                    }
                }
            }
        }

        file.seekg(0, ios::beg);
        file>>stockcode;

        //最后一天没有数据,说明当天停牌，不予计算
        if((data_set[data_set.rows_number -1][4] == 0)  || (data_set[data_set.rows_number -1][5] == 0)){
//           cout<<stockcode<<" stop today, and ignore it!"<<endl;
            toosmall = true;
            data_set.set(0,0);
        }
    }
    file.close();
    if(!toosmall)  {
        calculate_otherdata(needToCalculateAmount);
    }
}
//有时间限制的读入数据集
void SingleStock::load_dataset_date_constrained(
    string &path, int &year, int &month, int &day,
    unsigned int needToCalculateAmount){
    ifstream file(path.c_str());
    if(!file.is_open())
    {
        toosmall = true;
        cout << "Cannot open stock file.\n"<<endl;
        return;
    }
    timeUtil tu(year,month,day);
    // Set matrix sizes
    string line,ymd;
    getline(file, line);
    getline(file, line);                                    //去掉前两行
    rowsN = 0;
    while(file.good()){
        getline(file, line);
        if(line.size() > 20) {
            istringstream iss(line);
            iss>>ymd;
            if(tu.is_greater(ymd)){                         //指定日期大于等于记录日期，输出
                rowsN++;
            }
        }
    }
    if (rowsN > smallcretia){   toosmall = false;}

    // Clear file
    if(!toosmall){
        file.clear();
        file.seekg(0, ios::beg);
        getline(file, line);
        getline(file, line);                                //去掉前两行
        data_set.set(rowsN, attributes_count);
        date.resize(rowsN);
        if(is_time_statistics_on){  time_statistics.resize(rowsN);  }

        for(unsigned int i = 0; i < rowsN; i++)
        {
            //去掉原始数据中的时间
            for(unsigned int j = 0; j < raw_attributes_count+1; j++)
            {
                if(j > 0)
                    file >> data_set.data[i][j-1];
                else{
                    file>>line;
                    date[i] = line;
                    if(is_time_statistics_on){
                        time_statistics[i] = pharse_time_statistics(line);
                    }
                }
            }
        }

        file.seekg(0, ios::beg);
        file>>stockcode;

        //最后一天没有数据,说明当天停牌，不予计算
        if((data_set[data_set.rows_number -1][4] == 0)
            || (data_set[data_set.rows_number -1][5] == 0)){
 //           cout<<stockcode<<" stop today, and ignore it!"<<endl;
            toosmall = true;
            data_set.set(0,0);
        }
    }
    file.close();
    if(!toosmall)  {
        calculate_otherdata(needToCalculateAmount);
    }
}
SingleStock::SingleStock()
{
    results.resize(resultNum);
    scaled_results.resize(resultNum);
    toosmall = true;
    is_time_statistics_on = false;
}
void SingleStock::save(string filename){
    ofstream file(filename.c_str());
    if(!file.is_open()){return;}
    file << stockcode << endl;
    file << "date\topen\tmax\tmin\tclose\tamount\ttotalprice\tupratio\tvibratio" << endl;
    for(unsigned int i = 0; i < data_set.rows_number; i++){
        file << date[i] << "\t";
        for(unsigned int j = 0; j < data_set.columns_number; j++){
            file << data_set.data[i][j] << "\t";
        }
        file << std::endl;
    }
    file.close();
}
SingleStock::~SingleStock(void){}
ostream& operator<<(ostream& os, const SingleStock &ss){
    os<<ss.stockcode<<" raw: "<<ss.results<<" scaled: "<<ss.scaled_results;
    return os;
}
