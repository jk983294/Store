#ifndef MYSMARTPOINT_H
#define MYSMARTPOINT_H

#include <iostream>
#include <string>

template <class T>
class SmartPointer {
    T *ptr;
public:
    SmartPointer () : ptr (0) {}
    SmartPointer (T * p) : ptr (p) {}
    ~SmartPointer(){    delete ptr;}
    SmartPointer (const SmartPointer& mp)
    : ptr (new T (*mp.ptr)) {}
    SmartPointer& operator= (const SmartPointer& mp) {
        if (this  = &mp) {
            delete ptr;
            ptr = new T (*mp.ptr);
        }
        return *this;
    }
    T * operator-> () const {
        cout<<"operator->"<<endl;
        return ptr;
    }
    operator T* () const {
        cout<<"operator T* "<<endl;
        return ptr;
    }

};
struct Data{
    void print(){ cout<<"data"<<endl;}
};


template <class T> struct Content {
    T *ptr;
    int count;
    Content (T *p): ptr (p), count (1) {}
    ~Content (){
        delete ptr;
    }
};
template <class T>
class ManagedPointer {
Content <T> * body;
public:
    ManagedPointer (): body (0) {}
    ManagedPointer (T *p): body (new Content<T> (p)) {}
    ManagedPointer (const ManagedPointer& mp): body (mp.body) {
        if (body)
            ++body->count;
    }
    ManagedPointer& operator= (const ManagedPointer& mp){
        if (body != mp.body) {
            if (body && --body->count == 0) delete body;
            body = mp.body;
            if (body) ++body->count;
        }
        return *this;
    }
    ~ManagedPointer (){ if (body && --body->count == 0) delete body; }
    T& operator * () const{ return *body->ptr;}
    T* operator-> () const{ return body->ptr;}
    operator T * () const { return body->ptr;}
};



#endif // MYSMARTPOINT_H
