#ifndef CONFIGURE_H
#define CONFIGURE_H


//使用的算法
enum AlgorithmType{
	BrutalForce,					//蛮力法
	BranchAndBound,			//分支界限法
	DynamicProgramming	//动态规划
};
const AlgorithmType algotype = DynamicProgramming;

const bool inputHasWeight = false;				//测试用，边上带权值的txt格式问 [ v1,v2 weight ]
const int POSTIVE_MAXINT = 0x7fffffff;		//最大整数
const int MAX_NODE_NUM = 5000;				//最大节点数


#endif