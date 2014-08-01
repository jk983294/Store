#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>
#include <time.h>
#include <string>

using namespace std;

const int number_scope = 30;			// 1 2 ... 70
const int matrix_size = 50;
const int unit_capacity = 5;


//const int choice = 5;
class chromosome;

//单元格
class unit
{
public:
	unit(){
		count = unit_capacity;
		allocated = false;
	};
	void init(){
		count = unit_capacity;
		allocated = false;
	};

	int content[unit_capacity];
	int count;
	bool allocated;
	int limit;
	int vr;
};

class unit_info{
public:
	unit_info(){
		;
	};
	unit_info(int x,int y){
		i = x;
		j = y;
	};

	void set_i_j(int x,int y){
		i = x;
		j = y;
	};

	void init(int x, int y,chromosome *a);
public:
	int i;
	int j;
	int limit;
};

class sequence 
{
public:
	sequence();

	void exchange(int i,int j);

	void print(int vr,ofstream & order_outfile);

	int seq[matrix_size * matrix_size];

	int count;
};


class violate_record
{
public:
	violate_record(){
		level[0] = 0;
		level[1] = 0;
		level[2] = 0;
	}
	int level[3];
};

class chromosome
{
public:
	chromosome(){;};

	void init();

	void set_allocat_choice(int x , int y, int kind);	//分配allocat_choice  为单元格分配准备

	void refresh_allocat_choice(int num);				// 1级   开始分配该单元格调用  每分配一个数字调用一次，不需要循环

	int get_min_allocat_choice();						//获得当前allocat_choice最小代价选择

	void allocate_unit(int x,int y);					//分配一个单元格 x,y为矩阵坐标

	void normal_order();								//按从左到右  从上倒下的顺序分配单元格

	void best_order();

	void random_order();

	void count_limit();									//most_limit_first_order用

	void most_limit_first_order();

	void inward_order();

	void outward_order();

	void best_order1();

	void best_order2();

	void best_order3();

	void best_order4();

	void best_order5();

	void best_order6();

	void best_order7();

	void best_order8();

	void queue_order();

	void allocate_all(int choice);	

	void allocate_all_by_sequence( sequence* order );

	void set_vr_vector(int x,int y,int kind);			//初始化vr向量，为计算单个违约分准备

	void calculation_unit_violate_record(int x,int y);	//计算单个违约分

	void calculation_all_unit_violate_record();			//计算所有违约分

	void print_data();

public:
	unit unit_array[matrix_size][matrix_size];
	int total_vr;
};


class group
{
public:
	group(){	init(); };

	void init();				//随机生成100个的种群

	void select();				//选择  精英

	void crossover();			//交叉

	void mutate();				//变异

	void iteration();			//一次迭代

	void print_best();

	void print_all();

	void evolution();

public:
	vector<chromosome> chromosome_group;
	int group_count;
};