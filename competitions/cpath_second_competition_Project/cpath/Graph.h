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


class Node{	//�ڵ�
public:
	int id, weight;
public:
	Node(): id(0),weight(-1){};
	Node(int i,int w): id(i),weight(w){};
};
//��̬�滮�������м�����¼�ṹ
class dp_record{
public:
	int distance;							//���������ع��м�ڵ㣬���·������
	int pre_node_index;				//��¼�ý�����Ǳ߹��������ڻ�����ñ����м�ڵ��˳��
	set<int> mustpass;				//�Ѿ������ıع��ڵ�
	list<int> path;						//ֻ��¼·�����м�ڵ�
	bool is_has_path;					//�Ƿ���·��
public:
	dp_record(): distance(-1),pre_node_index(-1), is_has_path(false){};
	void set_data(int distance, list<int> &path);
};

class Graph
{
public:
    vector<list<Node> > linktable;											//�ڽӱ�
	vector<int> distance, parent;											//�ڵ㵽Դ�ڵ�ľ��� ��·���ڵ㵽Դ�ڵ㸸�׽ڵ� ����·���ڵ㵽Դ�ڵ㸸�׽ڵ�
	vector<int> mustpass_nodes, best_order;							//Դ��Ŀ�ġ��м���뾭���Ľڵ㣬��¼���˳��
	map<int, vector<int> > distancetable, parenttable;			//�����,�ڵ�v���������нڵ�ľ���
	int n,	source, dest ;															//������ NodeDisjoint�нڵ�ֲ�ǰ�ڵ���  Դ�ڵ� Ŀ�Ľڵ�
	int shortest_distance , middlecnt;										//������̾��� �м�ؾ��ڵ���
	list<int> mainpath;															//������·��
	set<pair<int, int> > to_prune;											//���<i,j>���·���а����бؾ��ڵ�,��Ҫ��֦
	set<int> middlenodes,allmustpass;									//�м���뾭���Ľڵ�(�����������յ�) ���бع��ڵ�
	vector<vector<dp_record> > Dij;										//D[i][j]��¼��i��j����̾��룬��·�����������κ������ؾ��ڵ�
	vector<vector<dp_record> > Fij;										//F[i][j]��¼�ڵ�j��dest����̾��룬��·���о�������i���м�ؾ��ڵ�
	bool is_no_path;																//�����ڸ�������·��						
public:
	Graph();
	~Graph();
	void calculate_path_pass_spefixed_nodes();								//�������

	//�������������е���ϱ���һ��
	void brutal_force();		

	//�������Ļ����ϣ����ϼ�֦�Ż�
	void branch_and_bound();																	
	void greedy_method_for_upbound();													//̰�ķ����һ�����н���Ϊ�Ͻ磬Ϊ��֦��׼��
	void organise_path_seg_by_seg();														//����best_order�ֶ����ƴ�ӳ���·��

	//��̬�滮
	void dynamic_programming();
	void initilize_dp_matrix();																	//��̬�滮��������ʼ��������󣬼�¼�ڵ��֮��ͨ�������ؾ��ڵ�
	void min_distance_through_exactly_k_nodes(int i,int ksi);					//�ڵ�i��destͨ��ksi���м�ڵ�����·����������������Fij[ksi][i]							
	void path_trace_back();																		//���ݶ�̬�滮�ļ�¼���ݳ����ű���˳��
	bool check_if_has_duplicate_nodes();												//�ö�̬�滮����ֻ�ܱ�֤�ؾ���ֻ����һ�Σ����ܱ�֤�ֶ�����·���������ظ���

	//utility
	void calculate_distance_table();															//�����м�ڵ㵽�������е�ľ���
	void insert_sort(vector<int> &array,int start,int end);						//��������������֦��ʱ���ض��������б�Ϊ�����
	void set_middle_nodes(vector<int> &middle);									//�������н����������м�ڵ���ϳɱؾ��ڵ�����mustpass_nodes


	//���·��
	void dijkstra_shortest_path(int root, bool isTerminateAsFound);			//ʹ��dijkstra�ҵ����·����isTerminateAsFoundָʾ�㷨�ҵ�����㵽�յ����ֹ
	void bfs_shortest_path(int root, bool isTerminateAsFound);				//BFS�ҳ���·����isTerminateAsFoundָʾ�㷨�ҵ�����㵽�յ����ֹ
	void dijkstra_without_pass_other_mustpass_nodes(int root);			//ʹ��dijkstra�ҵ���root��end�����·�����ҵ�����㵽�յ����ֹ
	void bfs_without_pass_other_mustpass_nodes(int root);				//BFS�ҵ���root��end�����·�����ҵ�����㵽�յ����ֹ

	//utility
	void resize(int capacity);													//����������С
	void add_edge(int v,int w,int weight, bool is_bilink);			//����ߣ�is_bilink��ʾ����˫���
	void set_edge(int v,int w,int weight);									//���ñ�Ȩֵ
	bool read_inputfile(string path);										//���������ļ�
	void print_result(string outputfile);									//���������ļ�
	void print_result(string description, list<int> &path);			//���·��������ն�
	void print_info();																//��ӡ������Ϣ
};


#endif


