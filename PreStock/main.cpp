#include <iostream>
#include "Configuration.h"
#include "SingleStock.h"
#include "AllStock.h"
#include "timeUtil.h"
#include "Strategy.h"
#include "Polyomial.h"


using namespace std;

void testSingle(){
    string path = "D:\\PreStockData\\RawData\\SZ002320.TXT";    //SZ000001.TXT
    SingleStock ss;
    ss.load_dataset(path,100);
    ss.calculate_all();
    ss.data_set.save("data.txt");
//    ss.saveAsCSVFormat();
}
void testSingle_date_constrained(){
    string path = "D:\\PreStockData\\RawData\\SZ000001.TXT";    //SZ000001.TXT
    SingleStock ss;
    int year=2013,  month = 5, day = 20;
    ss.load_dataset_date_constrained(path,year,month,day,100);
//    ss.calculate_all();
    ss.data_set.save("data.txt");
}

void testAll(){
    AllStock as(100);
    as.calculateAllByDirectory("D:\\PreStockData\\RawData\\");
    as.recommandTop(10);
    as.recordAllPrediction();
}
void testAll_date_constrained(){
    int year=2013,  month = 5, day = 24;
    cout<<"input last month day, and generate from that day past five days result"<<endl;
    cin>>month>>day;
    for(int i = 0; i < 5;i++){
        int lastday = day - i;
        AllStock as(100);
        as.calculate_all_directory_date_constrained("D:\\PreStockData\\RawData\\",year,month,lastday);
        as.recommandTop(10);
        as.recordAllPrediction();
    }

}
void single_simulation(){
    string path = "D:\\PreStockData\\RawData\\SZ002320.TXT";    //SZ002320.TXT SZ002606
    SingleStock ss;
    int need_to_calculate = 50;
    ss.load_dataset(path,need_to_calculate);
    ss.calculate_all();
//    ss.save("data.txt");

    Strategy stg(&ss);
    stg.simulation(ss.rowsN - need_to_calculate);

}
int main()
{

//    testSingle();
//    timeUtil tu;
//    cout<<tu.whichweekday(2013,5,23);
//    testAll();
//    testSingle_date_constrained();

//    testAll_date_constrained();

    single_simulation();

//  Polyomial::test();

    return 0;
}
