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
	int id, weight,residual_weight;
public:
	Node(): id(0),weight(-1),residual_weight(-1){};
	Node(int i,int w): id(i),weight(w),residual_weight(w){};
};

class Graph
{
public:
    vector<list<Node> > linktable;		                //���ӱ�
	vector<int> distance, parent;			//�ڵ㵽Դ�ڵ�ľ��� ��·���ڵ㵽Դ�ڵ㸸�׽ڵ� ����·���ڵ㵽Դ�ڵ㸸�׽ڵ�
	vector<int> primary_path,backup_path;		//��¼��·���ͱ���·��
	multimap<int,int> nonoverlap_edges;			//���ص���
	int n,		half_n,	source  , dest,	pathNum;		//������ NodeDisjoint�нڵ�ֲ�ǰ�ڵ���  Դ�ڵ� Ŀ�Ľڵ� ·����
	ConstrainTpye constraintype;						//�������� �߲��ظ�EdgeDisjoint �ڵ㲻�ظ�NodeDisjoint
public:
	Graph();
	~Graph();
	void calculate_all();																//���������

    //���ط���: ʹ��BFS�����·����Ȼ������������ɾ������һ��·�����ټ���ڶ���  
    void naive_record_primary_path();											//��¼��·��
    void naive_record_backup_path();											//��¼����·��
    void naive_calculate_backup_path_EdgeDisjoint();					//ɾ����·���ߺ����󱸷�·��
    void naive_calculate_backup_path_NodeDisjoint();				//ɾ����·���ڵ�����󱸷�·��
	void remove_edge(int v,int w, bool is_bilink_remove);			//ɾ����,isbilinkremoveɾ˫��ߣ�����ɾ�����
	void remove_node(int v);														//ɾ���ڵ��Լ��ڵ�����г������

	//������㷨: ʹ��Edmonds_Karp�����ҵ����е�����·��������·����������ߺ����飬ѡ����̵���������·��Ϊ����·��
	void Edmonds_Karp();															//����������·��
	void bfs_edmonds_karp();														//ʹ��BFS��һ������·��

	/*Suurballe�㷨������dijkstra�ҵ����·��P1,��ͼ�����б��޸�Ȩֵ��ʹ����������ȨֵΪ0��������Ȩֵ>=0
	   Ȼ�󹹽�ʣ��ͼ������dijkstra�����·��P2����P1��P2�ص��ķ����ɾ����P1��P2�ع�����·���ͱ���·��				*/
	void modify_weight_create_residual_graph();							//�޸�Ȩֵ������ʣ��ͼ,���߸�Ϊ����ָ��source�ı�ɾ��	
	void insert_and_erase_overlap_edge();									//�����ص��ķ����
	void reorganise_path();															//�����ص��ı�������֯��·��,ѡ��������̵���Ϊ����·��
	void split_vertices();																//ÿ���ڵ���Ϊ�����ڵ㣬��ڵ㵥�����ӵ����ڵ� (i, i+half_n) (���ڵ㣬��ڵ�)

	//���·��
	void dijkstra_shortest_path(bool isTerminateAsFound);			//ʹ��dijkstra�ҵ����·����isTerminateAsFoundָʾ�㷨�ҵ�����㵽�յ����ֹ
	void bfs_shortest_path(bool isTerminateAsFound);				//BFS�ҳ���·����isTerminateAsFoundָʾ�㷨�ҵ�����㵽�յ����ֹ

	//utility
	void resize(int capacity);													//����������С
	void add_edge(int v,int w,int weight, bool is_bilink);			//����ߣ�is_bilink��ʾ����˫���
	void set_edge(int v,int w,int weight);									//���ñ�Ȩֵ
	bool read_inputfile(string path);										//���������ļ�
	void print_result(string outputfile);									//���������ļ�
	list<Node>::iterator find_edge(int v,int w);						//�ұ�
	void remove_edge(int v,int w);											//ɾ��

	//debug
	void print_result(string description, vector<int> &path);	//���·��������ն�
	void print_info();																//��ӡ������Ϣ
	void visitNode(int current);												//��ӡ�ڵ���Ϣ
	void validate_correctness(string resultpath);						//��֤�ҵ��Ľ���Ƿ����������Լ��Ƿ���ͨ
	bool islinked(int v,int w);													//���ͼ���Ƿ��б�(v,w)
};


#endif


