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

//��Ԫ��
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

	void set_allocat_choice(int x , int y, int kind);	//����allocat_choice  Ϊ��Ԫ�����׼��

	void refresh_allocat_choice(int num);				// 1��   ��ʼ����õ�Ԫ�����  ÿ����һ�����ֵ���һ�Σ�����Ҫѭ��

	int get_min_allocat_choice();						//��õ�ǰallocat_choice��С����ѡ��

	void allocate_unit(int x,int y);					//����һ����Ԫ�� x,yΪ��������

	void normal_order();								//��������  ���ϵ��µ�˳����䵥Ԫ��

	void best_order();

	void random_order();

	void count_limit();									//most_limit_first_order��

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

	void set_vr_vector(int x,int y,int kind);			//��ʼ��vr������Ϊ���㵥��ΥԼ��׼��

	void calculation_unit_violate_record(int x,int y);	//���㵥��ΥԼ��

	void calculation_all_unit_violate_record();			//��������ΥԼ��

	void print_data();

public:
	unit unit_array[matrix_size][matrix_size];
	int total_vr;
};


class group
{
public:
	group(){	init(); };

	void init();				//�������100������Ⱥ

	void select();				//ѡ��  ��Ӣ

	void crossover();			//����

	void mutate();				//����

	void iteration();			//һ�ε���

	void print_best();

	void print_all();

	void evolution();

public:
	vector<chromosome> chromosome_group;
	int group_count;
};