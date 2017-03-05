#include "AllStock.h"
#include "timeUtil.h"
#include <io.h>

AllStock::AllStock(unsigned int needToCalculateAmount){
    this->needToCalculateAmount = needToCalculateAmount;
    is_date_constrained = false;
}
void AllStock::calculateAllByDirectory(string path ){
//  const char *to_search="D:\\PreStockData\\RawData\\*.*";         //欲查找的文件，支持通配符
    string tosearchpath = path+"*.TXT";

    long handle;                                                    //用于查找的句柄
    struct _finddata_t fileinfo;                                    //文件信息的结构体
    handle=_findfirst(tosearchpath.c_str(),&fileinfo);              //第一次查找
    if(handle == -1)  return ;

    cout<<"calculate all phase "<<endl;

    string filename(fileinfo.name);
    string filepath = path + filename;
    calculateSingle(filepath);
    while(!_findnext(handle,&fileinfo))                             //循环查找其他符合的文件，知道找不到其他的为止
    {
        string filename(fileinfo.name);
        string filepath = path + filename;
        calculateSingle(filepath);
    }
    _findclose(handle);
}
void AllStock::calculate_all_directory_date_constrained(string path,int &year,int &month,int &day){
    is_date_constrained = true;
    this->year = year;  this->month = month;  this->day = day;

    string tosearchpath = path+"*.TXT";
    long handle;                                                    //用于查找的句柄
    struct _finddata_t fileinfo;                                    //文件信息的结构体
    handle=_findfirst(tosearchpath.c_str(),&fileinfo);              //第一次查找
    if(handle == -1)  return ;

    cout<<"calculate all phase "<<endl;

    string filename(fileinfo.name);
    string filepath = path + filename;
    calculateSingle(filepath);
    while(!_findnext(handle,&fileinfo))                             //循环查找其他符合的文件，知道找不到其他的为止
    {

        string filename(fileinfo.name);
        string filepath = path + filename;
        calculateSingle(filepath);
    }
    _findclose(handle);
}
void AllStock::calculateSingle(string path){
    SingleStock ss;
    if(is_date_constrained){    ss.load_dataset_date_constrained(path,year,month,day,needToCalculateAmount);}
    else {  ss.load_dataset(path,needToCalculateAmount); }
    ss.calculate_all();
    ss.data_set.set(0,0);
    if(!ss.toosmall)
        allstock.push_back(ss);
}
void AllStock::resultFusion(){
    cout<<"result scale phase "<<endl;
    //规格化结果
    Matrix<double> ScaledResult;
    ScaledResult.set(allstock.size(),resultNum);
    for (unsigned int i = 0;i < allstock.size(); i++)
    {
        ScaledResult.set_row(i, allstock[i].results);
    }
    ScaledResult.scale_mean_standard_deviation_self();              //每列按均值方差缩放
    ScaledResult.scale_minimum_maximum_self();                      //每列按最大最小值缩放
    for (unsigned int i = 0;i < allstock.size(); i++)
    {
        for(unsigned int j = 0; j < resultNum; j++){
            allstock[i].scaled_results[j] = ScaledResult[i][j];
        }
    }


    cout<<"result fusion phase "<<endl;
    for (unsigned int i = 0;i < allstock.size(); i++)
    {
        allstock[i].resultFusion();
    }
}
//推荐最好的股票
void AllStock::recommandTop(int topnum){

    resultFusion();

    sort(allstock.begin() , allstock.end(), greater<SingleStock>());// 大到小

    int recomNum = (topnum < AllStock::stockNum) ? topnum : AllStock::stockNum;
    for (int i = 0; i < recomNum; i++) {
        cout <<i<<" "<<allstock[i]<< endl;
    }
}
//记录本次的所有预测
void AllStock::recordAllPrediction(){
    std::string filepath;
    if(is_date_constrained){
        ostringstream oss;
        oss<<"_dc_"<<month<<"_"<<day;
        filepath =  ResultPath +timeUtil::currentTime()+oss.str()+".txt";
    }
    else {
        filepath = ResultPath+timeUtil::currentTime()+".txt";
    }
    std::ofstream fout(filepath.c_str());
    int rank = 0;                                                   //预测结果排名

    fout <<ResultSpecfication<<endl;
    for (unsigned int i = 0;i < allstock.size(); i++)
    {
        fout <<rank<<" "<<allstock[i]<<endl;
        rank++;
    }
    fout.flush();
    fout.close();
}
