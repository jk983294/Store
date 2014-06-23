#include "StructDefine.h"
#include <fstream>
#include <stack>
#include <vector>
using namespace std;
/* 
* 每个键的左子树中的所有的键都小于这个键， 
* 每个键的右子树中的所有的键都大于等于这个键。 
* 叶子节点中的每个键都没有子树。 
*/ 

/* 当 M 等于 1 时也称为 2-3 树 
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
	int block_size;//记录块大小
	int conf_size;//头配置大小
	fstream file;//数据库关联文件
	fstream conf_file;//数据库配置文件
	configure	conf;//数据库头 配置信息
	node	current;//子记录块
	node    newnode; //用于临时存储结点
	node    deletenode;//用于临时存储结点
	node    parent;
	node    left_brother;
	node    right_brother;
	stack<int>  path;   //用于记录路径
	vector<int>  idle_block;   //删除记录的时候回收结点

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


	void createDB();//创建新的数据库

	void openDB();//打开配置文件

	void closeDB();//关闭数据库

	void read_block(int i);//读取记录快

	void write_block(int i);//写记录快

	void printDBconfig();//打印配置信息
	
	void printDBnode();//打印节点信息

	void printBtree();//打印B树
	
	void printrecord(record a);//打印记录

	void initNode();  //初始化结点

	bool get_path(int nu);   //获得路径

	int search(int number);                 //查找结点

	void searchScope(int a,int b);  //查找在a-b之间的电话号码

	void insert_value(record re);     //添加记录

	void del_value(int nu);        //删除记录

    int get_path1(int nu);   //获得路径

	bool isLeaf(int block); //判断是否是叶子结点

	void combine_node(node left,node parent,node right,int left_block,int parent_block,int right_block,int parent_pos);   //合并结点

	void split_node(record re,int blok,int left,int right);     //分离结点

	void insertRange(int range);

	
};

