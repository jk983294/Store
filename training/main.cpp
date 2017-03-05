#include <iostream>
#include "test.h"
#include "smartpointer.h"
#include <functional>
#include <algorithm>
#include <vector>

using namespace std;
using namespace std::placeholders;


void handle_body_ref_count();
void virtual_constructor();
void SmartPointer_test();
void functional();


int main()
{
    handle_body_ref_count();
    virtual_constructor();
    SmartPointer_test();
    functional();
    return 0;
}
void functional(){
    cout<<"------------------functional---------------------"<<endl;
    function<int(int,int)> myadd = [](int i,int j){return i+j;};
    function<int(int)> addby5 = std::bind(myadd, 1,2);  //bind to second param
    cout<<myadd(1,2)<<" "<<addby5(1)<<endl;

    int local1 =1,local2 = 2;
    auto f1 = [&local1,local2]{ local1++;};
    cout<<local1<<" "<<local2<<endl;                    //capture by ref changes

    vector<int> v{1,3,5,7,9,2,4,6,8,10};
    auto iter1 = find_if(v.begin(),v.end(),[](int x){ return x%2==0;});
    cout<<"first even element : "<<*iter1<<endl;

    function<function<int(int)>(int)> highorder = [](int i){
                                return [i](int j){ return i+j;};
                                };
    cout<<"high order function : "<<highorder(1)(3)<<endl;
}
void handle_body_ref_count(){
    cout<<"------------------handle_body_ref_count---------------------"<<endl;
    Handle a(1);
    Handle b(a);
    Handle c(b);
    Handle *d = new Handle(a);
    cout<<b.body->count<<endl;
}
void virtual_constructor(){
    cout<<"------------------virtual_constructor---------------------"<<endl;
    Base *b1 = new Derived1(1);
    Base *acopy = b1->clone();
    Base *b2 = Base::make ("Derived2");
    cout<<acopy->get_data()<<endl;
    cout<<b2->get_data()<<endl;
}
void SmartPointer_test()
{
    cout<<"------------------SmartPointer_test---------------------"<<endl;
    SmartPointer<Data> p (new Data());
    p -> print();                                       //call operator->
    (*p).print();                                       //call operator T*

    ManagedPointer<Data> p1(new Data());
    ManagedPointer<Data> p2 = p1;
    ManagedPointer<Data> p3(p2);
}
