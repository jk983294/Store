#ifndef SPARSEGRAPH_H
#define SPARSEGRAPH_H


#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <queue>
#include <stack>
#include <set>
#include <algorithm>
#include "configure.h"
using namespace std;


class Node{	//节点
public:
	int id, weight;
public:
	Node(): id(0),weight(-1){};
	Node(int i,int w): id(i),weight(w){};
};
//动态规划方法的中间结果记录结构
class dp_record{
public:
	int distance;							//经过几个必过中间节点，最短路径距离
	int pre_node_index;				//记录该结果从那边过来，便于回溯求得遍历中间节点的顺序
	set<int> mustpass;				//已经经过的必过节点
	list<int> path;						//只记录路径的中间节点
	bool is_has_path;					//是否有路径
public:
	dp_record(): distance(-1),pre_node_index(-1), is_has_path(false){};
	void set_data(int distance, list<int> &path);
};

class Graph
{
public:
    vector<list<Node> > linktable;											//邻接表
	vector<int> distance, parent;											//节点到源节点的距离 主路径节点到源节点父亲节点 备份路径节点到源节点父亲节点
	vector<int> mustpass_nodes, best_order;							//源、目的、中间必须经过的节点，记录最佳顺序
	map<int, vector<int> > distancetable, parenttable;			//距离表,节点v到其他所有节点的距离
	int n,	source, dest ;															//顶点数 NodeDisjoint中节点分拆前节点数  源节点 目的节点
	int shortest_distance , middlecnt;										//复赛最短距离 中间必经节点数
	list<int> mainpath;															//复赛中路径
	set<pair<int, int> > to_prune;											//组合<i,j>最短路径中包含有必经节点,需要剪枝
	set<int> middlenodes,allmustpass;									//中间必须经过的节点(不包括起点和终点) 所有必过节点
	vector<vector<dp_record> > Dij;										//D[i][j]记录从i到j的最短距离，该路径不允许经过任何其他必经节点
	vector<vector<dp_record> > Fij;										//F[i][j]记录节点j到dest的最短距离，该路径中经过正好i个中间必经节点
	bool is_no_path;																//不存在复赛的主路径						
public:
	Graph();
	~Graph();
	void calculate_path_pass_spefixed_nodes();								//计算入口

	//暴力法，将所有的组合遍历一遍
	void brutal_force();		

	//暴力法的基础上，加上剪枝优化
	void branch_and_bound();																	
	void greedy_method_for_upbound();													//贪心法求出一个可行解作为上界，为剪枝做准备
	void organise_path_seg_by_seg();														//根据best_order分段组合拼接成主路径

	//动态规划
	void dynamic_programming();
	void initilize_dp_matrix();																	//动态规划方法，初始化距离矩阵，记录节点对之间通过几个必经节点
	void min_distance_through_exactly_k_nodes(int i,int ksi);					//节点i到dest通过ksi个中间节点的最短路径，计算结果保存在Fij[ksi][i]							
	void path_trace_back();																		//根据动态规划的记录回溯出最优遍历顺序
	bool check_if_has_duplicate_nodes();												//该动态规划方法只能保证必经点只出现一次，不能保证分段最优路径不会有重复点

	//utility
	void calculate_distance_table();															//计算中间节点到其他所有点的距离
	void insert_sort(vector<int> &array,int start,int end);						//插入排序，用来剪枝的时候将特定区间排列变为最大序
	void set_middle_nodes(vector<int> &middle);									//从命令行解析出来的中间节点组合成必经节点向量mustpass_nodes


	//最短路径
	void dijkstra_shortest_path(int root, bool isTerminateAsFound);			//使用dijkstra找到最短路径，isTerminateAsFound指示算法找到从起点到终点就终止
	void bfs_shortest_path(int root, bool isTerminateAsFound);				//BFS找出主路径，isTerminateAsFound指示算法找到从起点到终点就终止
	void dijkstra_without_pass_other_mustpass_nodes(int root);			//使用dijkstra找到从root到end的最短路径，找到从起点到终点就终止
	void bfs_without_pass_other_mustpass_nodes(int root);				//BFS找到从root到end的最短路径，找到从起点到终点就终止

	//utility
	void resize(int capacity);													//调整容器大小
	void add_edge(int v,int w,int weight, bool is_bilink);			//插入边，is_bilink表示插入双向边
	void set_edge(int v,int w,int weight);									//设置边权值
	bool read_inputfile(string path);										//读入输入文件
	void print_result(string outputfile);									//结果输出到文件
	void print_result(string description, list<int> &path);			//结果路径输出到终端
	void print_info();																//打印容器信息
};


#endif


