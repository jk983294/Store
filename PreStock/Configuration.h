#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>

#include <windows.h>

#include "Vector.h"
#include "Matrix.h"
#include "timeUtil.h"
#include "MathUtil.h"

using namespace std;


enum PredictObject
{
    Strong,                                         //强势
    WeakReverse                                     //弱势反弹
};

const PredictObject predictobject = Strong;


//data set configuration
const unsigned int smallcretia = 200;               //数据集小于该阈值算小样本
const double InvalidValue = -100;                   //无效预测值
// 0       1     2    3    4     5    |||    6    7   8             9
// 开盘    最高  最低  收盘  成交量 成交额 |||  涨跌幅 振幅 短n日一阶趋势  长n日一阶趋势
const int raw_attributes_count = 6;                 //原始数据属性个数
const int new_attributes_count = 4;                 //后来计算的属性个数
const int attributes_count = raw_attributes_count + new_attributes_count;       //总属性个数
//0     长期量能活跃程度
//1     短期量能活跃程度
//2     长期振幅活跃程度
//3     短期振幅活跃程度
//4     长期振幅活跃程度
//5     短期振幅活跃程度
const unsigned int resultNum = 4;                   //结果数
const string ResultSpecfication = "长期量能  短期量能 长期振幅 短期振幅";


//原始数据路径
const std::string RawDataPath = "D:\\PreStockData\\RawDatadata\\";
//预测数据路径
const std::string ResultPath = "D:\\PreStockData\\result\\";

#endif
