#include <iostream>
#include "test.h"
#include "smartpointer.h"

//在for循环中，不要捕捉循环变量i，因为lambda是lazy eval的
//真正evaluate的时候循环变量i已经变化，可采用下面的方法将循环变量local化
//Thread lambda会捕获本地变量
//thread 当前状态