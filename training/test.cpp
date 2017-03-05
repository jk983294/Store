#include <iostream>
#include "test.h"
using namespace std;

map<string, Base *> Base::exemplars;
//向Base注册两个static factory
//必须保证在Base::exemplars初始化后注册
static Derived1 Derived1factory = Exemplar();
static Derived2 Derived2factory = Exemplar();



