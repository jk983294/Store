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
	int id, weight,residual_weight;
public:
	Node(): id(0),weight(-1),residual_weight(-1){};
	Node(int i,int w): id(i),weight(w),residual_weight(w){};
};

class Graph
{
public:
    vector<list<Node> > linktable;		                //连接表
	vector<int> distance, parent;			//节点到源节点的距离 主路径节点到源节点父亲节点 备份路径节点到源节点父亲节点
	vector<int> primary_path,backup_path;		//记录主路径和备份路径
	multimap<int,int> nonoverlap_edges;			//不重叠边
	int n,		half_n,	source  , dest,	pathNum;		//顶点数 NodeDisjoint中节点分拆前节点数  源节点 目的节点 路径数
	ConstrainTpye constraintype;						//限制类型 边不重复EdgeDisjoint 节点不重复NodeDisjoint
public:
	Graph();
	~Graph();
	void calculate_all();																//计算总入口

    //朴素方法: 使用BFS找最短路径，然后按照限制条件删除掉第一条路径，再计算第二条  
    void naive_record_primary_path();											//记录主路径
    void naive_record_backup_path();											//记录备份路径
    void naive_calculate_backup_path_EdgeDisjoint();					//删除主路径边后再求备份路径
    void naive_calculate_backup_path_NodeDisjoint();				//删除主路径节点后再求备份路径
	void remove_edge(int v,int w, bool is_bilink_remove);			//删除边,isbilinkremove删双向边，否则删单向边
	void remove_node(int v);														//删除节点以及节点的所有出边入边

	//最大流算法: 使用Edmonds_Karp方法找到所有的增广路径，增广路径消除反向边后重组，选择最短的两条增广路径为主备路径
	void Edmonds_Karp();															//找所有增广路径
	void bfs_edmonds_karp();														//使用BFS找一条增广路径

	/*Suurballe算法：先用dijkstra找到最短路径P1,将图中所有边修改权值，使得生成树的权值为0，其他边权值>=0
	   然后构建剩余图，再用dijkstra找最短路径P2，将P1和P2重叠的反向边删除，P1和P2重构成主路径和备份路径				*/
	void modify_weight_create_residual_graph();							//修改权值，构建剩余图,树边改为反向，指向source的边删除	
	void insert_and_erase_overlap_edge();									//消除重叠的反向边
	void reorganise_path();															//将不重叠的边重新组织成路径,选择两条最短的作为主备路径
	void split_vertices();																//每个节点拆分为两个节点，入节点单向连接到出节点 (i, i+half_n) (出节点，入节点)

	//最短路径
	void dijkstra_shortest_path(bool isTerminateAsFound);			//使用dijkstra找到最短路径，isTerminateAsFound指示算法找到从起点到终点就终止
	void bfs_shortest_path(bool isTerminateAsFound);				//BFS找出主路径，isTerminateAsFound指示算法找到从起点到终点就终止

	//utility
	void resize(int capacity);													//调整容器大小
	void add_edge(int v,int w,int weight, bool is_bilink);			//插入边，is_bilink表示插入双向边
	void set_edge(int v,int w,int weight);									//设置边权值
	bool read_inputfile(string path);										//读入输入文件
	void print_result(string outputfile);									//结果输出到文件
	list<Node>::iterator find_edge(int v,int w);						//找边
	void remove_edge(int v,int w);											//删边

	//debug
	void print_result(string description, vector<int> &path);	//结果路径输出到终端
	void print_info();																//打印容器信息
	void visitNode(int current);												//打印节点信息
	void validate_correctness(string resultpath);						//验证找到的结果是否满足限制以及是否连通
	bool islinked(int v,int w);													//检测图中是否有边(v,w)
};


#endif


