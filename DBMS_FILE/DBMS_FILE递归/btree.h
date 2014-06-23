#include "StructDefine.h"
#include <fstream>
#include <stack>
#include <vector>
using namespace std;
/* 
* ÿ�������������е����еļ���С��������� 
* ÿ�������������е����еļ������ڵ���������� 
* Ҷ�ӽڵ��е�ÿ������û�������� 
*/ 

/* �� M ���� 1 ʱҲ��Ϊ 2-3 �� 
* +----+----+ 
* | k0 | k1 | 
* +-+----+----+--- 
* | p0 | p1 | p2 | 
* +----+----+----+ 
*/ 


class BTree{

public:
	char *filename;
	char *conffilename;
	int block_size;//��¼���С
	int conf_size;//ͷ���ô�С
	fstream file;//���ݿ�����ļ�
	fstream conf_file;//���ݿ������ļ�
	configure	conf;//���ݿ�ͷ ������Ϣ
	node	current;//�Ӽ�¼��
	node    newnode; //������ʱ�洢���
	node    deletenode;//������ʱ�洢���
	node    parent;
	node    left_brother;
	node    right_brother;
	stack<int>  path;   //���ڼ�¼·��
	vector<int>  idle_block;   //ɾ����¼��ʱ����ս��

	BTree(char* a,char* b){
		filename=a;
		conffilename = b;
		block_size = sizeof(node);
		conf_size = sizeof(configure);
		idle_block.clear();
		//path.clear();
		if (!path.empty())
		{
			path.pop();
		}
	};
	~BTree(){
		closeDB();
	};


	void createDB();//�����µ����ݿ�

	void openDB();//�������ļ�

	void closeDB();//�ر����ݿ�

	void read_block(int i);//��ȡ��¼��

	void write_block(int i);//д��¼��

	void printDBconfig();//��ӡ������Ϣ
	
	void printDBnode();//��ӡ�ڵ���Ϣ

	void printBtree();//��ӡB��
	
	void printrecord(record a);//��ӡ��¼

	void initNode();  //��ʼ�����

	bool get_path(int nu);   //���·��

	int search(int number);                 //���ҽ��

	void searchScope(int a,int b);  //������a-b֮��ĵ绰����

	void insert_value(record re);     //��Ӽ�¼

	void del_value(int nu);        //ɾ����¼

    int get_path1(int nu);   //���·��

	bool isLeaf(int block); //�ж��Ƿ���Ҷ�ӽ��

	void combine_node(node left,node parent,node right,int left_block,int parent_block,int right_block,int parent_pos);   //�ϲ����

	void split_node(record re,int blok,int left,int right);     //������

	void insertRange(int range);

	
};

