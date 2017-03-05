#ifndef TEST_H
#define TEST_H

#include <iostream>
#include <map>
#include <string>

using namespace std;

class Body {                                //都是私有成员，只对友元类Handle开放
public:
    int data;
    int count;                              //引用计数
    Body (int d) : data (d), count(1) {}
    ~Body () {}
    friend class Handle;
};
class Handle
{
public:
    Body *body;
    Handle (int i = 0) : body (new Body (i)) {}
    Handle (const Handle& h) : body(h.body)
    { ++body->count;}                       //拷贝时增加计数器即可，共享一个Handle数据
    Handle& operator= (const Handle& h) {
        if (body != h.body) {               //两人的Handle数据不同
            if (--body->count == 0)
                delete body;                //我是该资源的最后持有人，负责析构
            body = h.body;
            ++body->count;
        }
        return *this;
    }
    ~Handle() {                             //我是该资源的最后持有人，负责析构
        if (--body->count == 0) delete body;
    }
    int get () const { return body->data; }
    void set (int i) {                      //copy on write
        if (body->count == 1)               //没有其他人用该数据直接修改
            body->data = i;
        else {                              //有人使用该数据则必须拷贝数据后进行修改，保证原数据不变
            --body->count;
            body = new Body (i);
        }
    }
};


class Exemplar {};                          //提供register时的正确CTOR版本
class Base {
public:
    int data;
    Base (int data_ = 0):data(data_){}
    virtual ~Base (){}
    virtual int get_data () const{ return data;}
    //由每个subclass自己提供具体类的拷贝实现
    virtual Base * clone () const = 0;
    //每个subclass提供构造自己的方法
    virtual Base * build () const = 0 ;
    //client code由此去按需求生成相应的object
    static Base * make (const string& type){
        return ((Base::exemplars)[type])->build();
    }
    //每个subclass将自己的一个实例注册到exemplars中,由lib提供者实现
    static void Register (string type,   Base *e){ exemplars[type]=e; }
public:
    static map<string,  Base *> exemplars;
};
class Derived1 : public Base {
public:
    Derived1 (int data) : Base(data){}
    ~Derived1 (){}
    int get_data () const{ return data + 1;}
    //subclass 提供自己类型的拷贝函数
    Derived1 * clone () const { return new Derived1 (*this); }
    //该版本CTOR将自己向base注册
    Derived1 (Exemplar){    Base::Register ("Derived1",this);     }
    //subclass 提供自己类型的虚构造函数
    Derived1 * build () const{ return new Derived1(0);}
};
class Derived2 : public Base {
public:
    Derived2 (int data) : Base(data){}
    ~Derived2 (){}
    int get_data () const{ return data + 2;}
    //subclass 提供自己类型的拷贝函数
    Derived2 * clone () const { return new Derived2 (*this); }
    //该版本CTOR将自己向base注册
    Derived2 (Exemplar){    Base::Register ("Derived2",this);     }
    //subclass 提供自己类型的虚构造函数
    Derived2 * build () const{ return new Derived2(0);}
};

#endif // TEST_H
