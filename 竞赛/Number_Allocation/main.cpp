#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

ofstream outfile("out.txt");


const int number_scope = 30;			// 1 2 ... 70
const int matrix_size = 4;
const int unit_capacity = 5;
// 0: normal_order 1: most_limit_first_order 2: inward_order 3: outward_order 4:best 5 best1
//const int choice = 4;					//

int allocat_choice[number_scope+2];	// 0 1 2 .... 70 71  ������������  ���÷�Χ�ж�

int total_vr = 0;

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


unit unit_array[matrix_size][matrix_size];

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
/************************************************************************/
/* ͨ���Ըõ�Ԫ����Χ��25����Ԫ�񣬼���һ����Ԫ�������ֵ               */
/************************************************************************/
	void init(int x, int y){
		i = x;
		j = y;
		int index1,index2,limitation;
		limitation = 0;
		for (index1 = x - 2; index1 <= x + 2; index1++)
		{
			for (index2 = y -2; index2 <= y + 2; index2++)
			{
				if ((index1 >= 0) && (index1 < matrix_size) && (index2 >= 0) && (index2 < matrix_size))//�Ϸ���
				{
					if (abs(index1 - x)==2 || abs(index2 - y)==2)//3��
					{
						limitation += unit_array[index1][index2].count;
					}
					else if (abs(index1 - x)==1 || abs(index2 - y)==1)//2��
					{
						limitation += unit_array[index1][index2].count * 10;
					}
					else {
						//1��
						limitation += unit_array[index1][index2].count * 50;
					}
				}
			}
		}
		limit = limitation;
		unit_array[x][y].limit = limitation;
	};
	int i;
	int j;
	int limit;
};

vector<unit_info> unit_info_vector(matrix_size * matrix_size);

inline bool cmp_data(const unit_info &left, const unit_info &right)
{ 
	return (left.limit > right.limit) ? true : false ; //��С
	//return (left.limit >= right.limit) ? false : true ;//С����
}

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
violate_record vr[number_scope+2];
/************************************************************************/
/* ����ÿ����Ԫ�е������                                               */
/************************************************************************/
void randint(){
	ifstream input("randint.txt");
	int index1,index2;
	for (index1 =0; index1 <matrix_size; index1++)
	{
		for (index2 =0; index2 <matrix_size; index2++)
		{
			input>>unit_array[index1][index2].count;
		}
	}
	input.close();
}
/************************************************************************/
/* ����ÿ����Ԫ����Χ������ֵ���������򣬴����Ƶ�Ԫ������               */
/************************************************************************/
void count_limit(){
	int index1,index2;
	for (index1 =0; index1 <matrix_size; index1++)
	{
		for (index2 =0; index2 <matrix_size; index2++)
		{
			unit_info_vector[index1*matrix_size + index2].init(index1,index2);
		}
	}
	stable_sort(unit_info_vector.begin(), unit_info_vector.end(), cmp_data);
}

/************************************************************************/
/* ����allocat_choice  Ϊ��Ԫ�����׼��                                 */
/************************************************************************/
void set_allocat_choice(int x , int y, int kind){
	if (unit_array[x][y].allocated)
	{
		int index,num;
		for (index = 0;index < unit_array[x][y].count;index ++ )//���������ڳ�ʼ׼��ʱ����
		{
			num = unit_array[x][y].content[index];
			if (kind == 3)//3��
			{
				allocat_choice[num] += 1;
			}
			else if (kind == 2)//2��
			{
				allocat_choice[num] += 20;
				allocat_choice[num - 1] += 10;
				allocat_choice[num + 1] += 10;
			}
		}
	}	
}
/************************************************************************/
/*1��   ��ʼ����õ�Ԫ�����  ÿ����һ�����ֵ���һ�Σ�����Ҫѭ��        */
/************************************************************************/
void refresh_allocat_choice(int num){
	allocat_choice[num] += 100;
	allocat_choice[num - 1] += 50;
	allocat_choice[num + 1] += 50;
}
/************************************************************************/
/* ��õ�ǰallocat_choice��С����ѡ��                                   */
/************************************************************************/
int get_min_allocat_choice(){
	int index,min_index,min_value;
	min_index = 1;
	min_value = allocat_choice[min_index];
	for (index = 1; index <= 70; index++)
	{
		if (allocat_choice[min_index] > allocat_choice[index])
		{
			min_index = index;
			min_value = allocat_choice[index];
		}
		if (allocat_choice[min_index] == 0)//�Ѿ���С��
		{
			return min_index;
		}
	}
	return min_index;
}
/************************************************************************/
/* ����һ����Ԫ�� x,yΪ��������                                         */
/************************************************************************/
void allocate_unit(int x,int y){

	//outfile<<x<<"_"<<y<<" ";

	//����
	memset(allocat_choice,0,sizeof(int)*(number_scope + 2));
	int index1,index2,min_index;
	for (index1 = x - 2; index1 <= x + 2; index1++)
	{
		for (index2 = y -2; index2 <= y + 2; index2++)
		{
			if ((index1 >= 0) && (index1 < matrix_size) && (index2 >= 0) && (index2 < matrix_size))//�Ϸ���
			{
				if (abs(index1 - x)==2 || abs(index2 - y)==2)//3��
				{
					set_allocat_choice(index1,index2,3);
				}
				else if (abs(index1 - x)==1 || abs(index2 - y)==1)//2��
				{
					set_allocat_choice(index1,index2,2);
				}
			}
		}
	}

	//��ʼ����
	for (index1 = 0;index1 < unit_array[x][y].count;index1 ++ )
	{
		min_index = get_min_allocat_choice();
		unit_array[x][y].content[index1] = min_index;
		refresh_allocat_choice(min_index);	//ˢ����С
	}

	unit_array[x][y].allocated = true;
}
/************************************************************************/
/*   �ܷ���������  ��������  ���ϵ��µ�˳����䵥Ԫ��               */
/************************************************************************/
void normal_order(){
	int index1,index2;
	//����˳��
	for (index1 =0; index1 <matrix_size; index1++)
	{
		for (index2 =0; index2 <matrix_size; index2++)
		{
			allocate_unit(index1,index2);
		}
	}
}
/************************************************************************/
/* �ܷ���������  �����������                                         */
/************************************************************************/
void most_limit_first_order(){
	count_limit();
	vector <unit_info>::iterator v1_Iter;
    for ( v1_Iter = unit_info_vector.begin( ) ; v1_Iter != unit_info_vector.end( ) ; v1_Iter++ ){
		allocate_unit(v1_Iter->i,v1_Iter->j);
		outfile<<v1_Iter->i<<"_"<<v1_Iter->j<<"_"<<v1_Iter->limit<<" ";
	}
	outfile<<endl;
}
/************************************************************************/
/* �ܷ��������� ��������ڲ����                                      */
//    ------------->y  ���ά����array[x][y]����һ��
//    |
//    |
//    |
//    |
//    |
//    (x) 
/************************************************************************/
void inward_order(){
	int direction = 0; // 0 1 2 3 : �� �� �� ��
	int x,y,tmp_x,tmp_y;
	x = 0;
	y = 0;
	while (1)
	{
		outfile<<x<<"_"<<y<<" ";
		allocate_unit(x,y);	

		//���յ�ǰ���� �ܲ�����ǰ�ߣ�����������ߣ������������ַ���ת����
		switch(direction){
		case 0:{
			tmp_y = y + 1;
			tmp_x = x ;
			break;
			   }
		case 1:{
			tmp_y = y ;
			tmp_x = x + 1;
			break;
			   }
		case 2:{
			tmp_y = y - 1;
			tmp_x = x ;
			break;
			   }
		case 3:{
			tmp_y = y;
			tmp_x = x - 1;
			break;
			   }
		}

		if ((tmp_x >= 0) && (tmp_x < matrix_size) 
			&& (tmp_y >= 0) && (tmp_y < matrix_size)
			&& (!unit_array[tmp_x][tmp_y].allocated))//�߽�Ϸ��� �õ�Ԫ��û�б�����
		{
			x = tmp_x;
			y = tmp_y;
			continue;
		}
		else { //�����ַ���ת����
			direction = (direction + 1) % 4 ;
			//�����·���  
			switch(direction){
			case 0:{
				y++;
				break;
				   }
			case 1:{
				x++;
				break;
				   }
			case 2:{
				y--;
				break;
				   }
			case 3:{
				x--;
				break;
				   }
			}
			//�·����һ����Ԫ�����Ѿ���������ȫ�������
			if (unit_array[x][y].allocated)
			{
				break;
			}
		}
	}
	outfile<<endl;
}
/************************************************************************/
/*  �ܷ��������� ���ڲ���������                                     */
/************************************************************************/
void outward_order(){
	int direction = 3; // 0 1 2 3 : �� �� �� ��
	int tmp_direction;
	int x,y,tmp_x,tmp_y;

	x = matrix_size/2;
	y = matrix_size/2;
	allocate_unit(x,y);//�����ȷ���һ��
	
	y--;
	while (1)
	{
		outfile<<x<<"_"<<y<<" ";
		allocate_unit(x,y);	

		tmp_direction = (direction + 1) % 4 ;
		//�����·���  
		switch(tmp_direction){
		case 0:{
			tmp_y = y + 1;
			tmp_x = x ;
			break;
			   }
		case 1:{
			tmp_y = y ;
			tmp_x = x + 1;
			break;
			   }
		case 2:{
			tmp_y = y - 1;
			tmp_x = x ;
			break;
			   }
		case 3:{
			tmp_y = y;
			tmp_x = x - 1;
			break;
			   }
		}
		//�·����һ����Ԫ����û�б����������·�����
		if (!unit_array[tmp_x][tmp_y].allocated)
		{
			x = tmp_x;
			y = tmp_y;
			direction = tmp_direction;
			continue;
		}
		else { //���Ѿ����������վɷ�����
			switch(direction){
			case 0:{
				y++;
				break;
				   }
			case 1:{
				x++;
				break;
				   }
			case 2:{
				y--;
				break;
				   }
			case 3:{
				x--;
				break;
				   }
			}
			if ((x < 0) || (x >= matrix_size) 
				|| (y < 0) || (y >= matrix_size))//�����߽���ȫ�������
			{
				break;	
			}
		}
	}
	outfile<<endl;
}
/************************************************************************/
/* stack order                                                                     */
/************************************************************************/
void queue_order(){
	queue<unit_info> q;

	int index,index1;
	int half = matrix_size/2;

	unit_info upleft(half-1 ,half-1);
	unit_info upright(half-1 ,half);
	unit_info downleft(half ,half-1);
	unit_info downright(half ,half);
	allocate_unit(upleft.i,upleft.j);
	q.push(upleft);
	unit_info temp,temp1;

	while(!q.empty()){
         temp = q.front();

		 if (temp.j > 0 && (!unit_array[temp.i][temp.j - 1].allocated))
		 {
			temp1.i = temp.i;
			temp1.j = temp.j -1;
			allocate_unit(temp1.i,temp1.j);
			q.push(temp1);
		 }
		 if (temp.i > 0 && (!unit_array[temp.i - 1][temp.j].allocated))
		 {
			 temp1.i = temp.i - 1;
			 temp1.j = temp.j ;
			 allocate_unit(temp1.i,temp1.j);
			 q.push(temp1);
		 }
		 if ((temp.j < matrix_size -1) && (!unit_array[temp.i][temp.j + 1].allocated))
		 {
			 temp1.i = temp.i;
			 temp1.j = temp.j + 1;
			 allocate_unit(temp1.i,temp1.j);
			 q.push(temp1);
		 }
		 if ((temp.i < matrix_size - 1) && (!unit_array[temp.i + 1][temp.j ].allocated))
		 {
			 temp1.i = temp.i + 1;
			 temp1.j = temp.j ;
			 allocate_unit(temp1.i,temp1.j);
			 q.push(temp1);
		 }

         q.pop();
    }
}
/************************************************************************/
/* best order                                                           */
/************************************************************************/
void best_order(){
	int index,index1;
	int half = matrix_size/2;

	unit_info upleft(half-1 ,half-1) ;
	unit_info upright(half-1 ,half);
	unit_info downleft(half ,half-1);
	unit_info downright(half ,half);

	for (index = 0; index < half; index++)
	{
		for (index1 = upleft.j + 1; index1 < upright.j;index1++){			
			allocate_unit(upleft.i,index1);
			outfile<<upleft.i<<"_"<<index1<<" ";
		}
		for (index1 = upright.i + 1; index1 < downright.i;index1++){
			allocate_unit(index1,upright.j);
			outfile<<index1<<"_"<<upright.j<<" ";
		}
		for (index1 = downright.j - 1; index1 > downleft.j;index1--){
			allocate_unit(downleft.i,index1);
			outfile<<downleft.i<<"_"<<index1<<" ";
		}
		for (index1 = downleft.i - 1; index1 > upleft.i;index1--){
			allocate_unit(index1 , upleft.j);
			outfile<<index1<<"_"<<upleft.j<<" ";
		}

		allocate_unit(upleft.i,upleft.j);
		allocate_unit(upright.i,upright.j);
		allocate_unit(downleft.i,downleft.j);
		allocate_unit(downright.i,downright.j);

		outfile<<upleft.i<<"_"<<upleft.j<<" ";
		outfile<<upright.i<<"_"<<upright.j<<" ";
		outfile<<downleft.i<<"_"<<downleft.j<<" ";
		outfile<<downright.i<<"_"<<downright.j<<" ";

		upleft.set_i_j(upleft.i - 1,upleft.j - 1);
		upright.set_i_j(upright.i - 1,upright.j + 1);
		downleft.set_i_j(downleft.i + 1,downleft.j - 1);
		downright.set_i_j(downright.i + 1,downright.j + 1);
		
	}
}

/************************************************************************/
/* best order                                                           */
/************************************************************************/
void best_order4(){
	int index,index1;
	int half = matrix_size/2;

	unit_info upleft(half-1 ,half-1) ;
	unit_info upright(half-1 ,half);
	unit_info downleft(half ,half-1);
	unit_info downright(half ,half);

	for (index = 0; index < half; index++)
	{
		for (index1 = upleft.j ; index1 < upright.j;index1++){			
			allocate_unit(upleft.i,index1);

		}
		for (index1 = downright.j ; index1 > downleft.j;index1--){
			allocate_unit(downleft.i,index1);
		}
		for (index1 = upright.i ; index1 < downright.i;index1++){
			allocate_unit(index1,upright.j);
		}
		for (index1 = downleft.i ; index1 > upleft.i;index1--){
			allocate_unit(index1,upleft.j);
		}
		upleft.set_i_j(upleft.i - 1,upleft.j - 1);
		upright.set_i_j(upright.i - 1,upright.j + 1);
		downleft.set_i_j(downleft.i + 1,downleft.j - 1);
		downright.set_i_j(downright.i + 1,downright.j + 1);
	}
}
/************************************************************************/
/* best order                                                           */
/************************************************************************/
void best_order5(){
	int index,index1;
	int half = matrix_size/2;

	unit_info upleft(half-1 ,half-1) ;
	unit_info upright(half-1 ,half);
	unit_info downleft(half ,half-1);
	unit_info downright(half ,half);

	for (index = 0; index < half; index++)
	{
		for (index1 = upleft.i ; index1 < downleft.i;index1++){
			allocate_unit(index1,downleft.j);
		}
		for (index1 = downright.i ; index1 > upright.i;index1--){
			allocate_unit(index1,upright.j);
		}
		for (index1 = downleft.j ; index1 < downright.j;index1++){			
			allocate_unit(downleft.i,index1);
		}		
		for (index1 = upright.j ; index1 > upleft.j;index1--){
			allocate_unit(upleft.i,index1);
		}

		upleft.set_i_j(upleft.i - 1,upleft.j - 1);
		upright.set_i_j(upright.i - 1,upright.j + 1);
		downleft.set_i_j(downleft.i + 1,downleft.j - 1);
		downright.set_i_j(downright.i + 1,downright.j + 1);

	}
}
/************************************************************************/
/* best order                                                           */
/************************************************************************/
void best_order6(){
	int index,index1;
	int half = matrix_size/2;

	unit_info upleft(half-1 ,half-1);
	unit_info upright(half-1 ,half);
	unit_info downleft(half ,half-1);
	unit_info downright(half ,half);

	for (index = 0; index < half; index++)
	{
		if (index % 2 == 0)
		{
			for (index1 = upleft.j ; index1 < upright.j;index1++){			
				allocate_unit(upleft.i,index1);

			}
			for (index1 = downright.j ; index1 > downleft.j;index1--){
				allocate_unit(downleft.i,index1);
			}
			for (index1 = upright.i ; index1 < downright.i;index1++){
				allocate_unit(index1,upright.j);
			}
			for (index1 = downleft.i ; index1 > upleft.i;index1--){
				allocate_unit(index1,upleft.j);
			}
		}
		else{
			for (index1 = upleft.i ; index1 < downleft.i;index1++){
				allocate_unit(index1,downleft.j);
			}
			for (index1 = downright.i ; index1 > upright.i;index1--){
				allocate_unit(index1,upright.j);
			}
			for (index1 = downleft.j ; index1 < downright.j;index1++){			
				allocate_unit(downleft.i,index1);
			}		
			for (index1 = upright.j ; index1 > upleft.j;index1--){
				allocate_unit(upleft.i,index1);
			}
			

		}

		upleft.set_i_j(upleft.i - 1,upleft.j - 1);
		upright.set_i_j(upright.i - 1,upright.j + 1);
		downleft.set_i_j(downleft.i + 1,downleft.j - 1);
		downright.set_i_j(downright.i + 1,downright.j + 1);

	}
}

void best_order7(){
	int index,index1;
	int half = matrix_size/2;

	unit_info upleft(half-1 ,half-1) ;
	unit_info upright(half-1 ,half);
	unit_info downleft(half ,half-1);
	unit_info downright(half ,half);

	for (index = 0; index < half; index++)
	{
		if (index %2 == 0)
		{
			for (index1 = downright.i - 1; index1 > upright.i;index1--){
				allocate_unit(index1,downright.j);
			}
			for (index1 = upright.j - 1; index1 >upleft.j;index1--){
				allocate_unit(upright.i,index1);
			}
			for (index1 = upleft.i + 1; index1 < downleft.i;index1++){			
				allocate_unit(index1,upleft.j);
			}
			for (index1 = downleft.j + 1; index1 < downright.j;index1++){
				allocate_unit(downleft.i,index1);
			}
			allocate_unit(upleft.i,upleft.j);
			allocate_unit(upright.i,upright.j);
			allocate_unit(downleft.i,downleft.j);
			allocate_unit(downright.i,downright.j);			
		}
		else{
			for (index1 = upleft.j + 1; index1 < upright.j;index1++){			
				allocate_unit(upleft.i,index1);
			}
			for (index1 = upright.i + 1; index1 < downright.i;index1++){
				allocate_unit(index1,upright.j);
			}
			for (index1 = downright.j - 1; index1 > downleft.j;index1--){
				allocate_unit(downleft.i,index1);
			}
			for (index1 = downleft.i - 1; index1 > upleft.i;index1--){
				allocate_unit(index1 , upleft.j);
			}
			allocate_unit(downright.i,downright.j);
			allocate_unit(upright.i,upright.j);
			allocate_unit(upleft.i,upleft.j);			
			allocate_unit(downleft.i,downleft.j);			
		}
		upleft.set_i_j(upleft.i - 1,upleft.j - 1);
		upright.set_i_j(upright.i - 1,upright.j + 1);
		downleft.set_i_j(downleft.i + 1,downleft.j - 1);
		downright.set_i_j(downright.i + 1,downright.j + 1);
	}
}

/************************************************************************/
/* best order   3                                                        */
/************************************************************************/
void best_order3(){
	int index,index1;
	int half = matrix_size/2;

	unit_info upleft(half-1 ,half-1) ;
	unit_info upright(half-1 ,half);
	unit_info downleft(half ,half-1);
	unit_info downright(half ,half);

	for (index = 0; index < half; index++)
	{
		for (index1 = upleft.j ; index1 <= upright.j;index1++){			
			allocate_unit(upleft.i,index1);

		}
		for (index1 = upright.i + 1; index1 <= downright.i;index1++){
			allocate_unit(index1,upright.j);

		}
		for (index1 = downright.j - 1; index1 >= downleft.j;index1--){
			allocate_unit(downleft.i,index1);

		}
		for (index1 = downleft.i - 1; index1 > upleft.i;index1--){
			allocate_unit(index1 , upleft.j);

		}
// 		allocate_unit(upleft.i,upleft.j);
// 		allocate_unit(upright.i,upright.j);
// 		allocate_unit(downleft.i,downleft.j);
// 		allocate_unit(downright.i,downright.j);
		upleft.set_i_j(upleft.i - 1,upleft.j - 1);
		upright.set_i_j(upright.i - 1,upright.j + 1);
		downleft.set_i_j(downleft.i + 1,downleft.j - 1);
		downright.set_i_j(downright.i + 1,downright.j + 1);

	}
}
/************************************************************************/
/* best order  1                                                         */
/************************************************************************/
void best_order1(){
	int index,index1;
	int half = matrix_size/2;

	unit_info upleft(half-1 ,half-1) ;
	unit_info upright(half-1 ,half);
	unit_info downleft(half ,half-1);
	unit_info downright(half ,half);

	for (index = 0; index < half; index++)
	{
		for (index1 = 0; index1 < half -1 - upleft.j ;index1++)
		{
			allocate_unit(upleft.i,half - 1 - index1);
			outfile<<upleft.i<<"_"<<(half - 1 - index1)<<" ";
			allocate_unit(upleft.i,half + index1);
			outfile<<upleft.i<<"_"<<(half + index1)<<" ";

			allocate_unit(half - 1 - index1 , upright.j);
			outfile<<(half - 1 - index1)<<"_"<<(upright.j)<<" ";
			allocate_unit(half + index1 , upright.j);
			outfile<<(half + index1)<<"_"<<(upright.j)<<" ";

			allocate_unit(downleft.i,half - 1 - index1);
			outfile<<downleft.i<<"_"<<(half - 1 - index1)<<" ";
			allocate_unit(downleft.i,half + index1);
			outfile<<downleft.i<<"_"<<(half + index1)<<" ";

			allocate_unit(half - 1 - index1 , downleft.j);
			outfile<<(half - 1 - index1)<<"_"<<(downleft.j)<<" ";
			allocate_unit(half + index1 , downleft.j);
			outfile<<(half + index1)<<"_"<<(downleft.j)<<" ";
		}

		allocate_unit(upleft.i,upleft.j);
		allocate_unit(upright.i,upright.j);
		allocate_unit(downright.i,downright.j);
		allocate_unit(downleft.i,downleft.j);


		outfile<<upleft.i<<"_"<<upleft.j<<" ";
		outfile<<upright.i<<"_"<<upright.j<<" ";
		outfile<<downleft.i<<"_"<<downleft.j<<" ";
		outfile<<downright.i<<"_"<<downright.j<<" ";

		upleft.set_i_j(upleft.i - 1,upleft.j - 1);
		upright.set_i_j(upright.i - 1,upright.j + 1);
		downright.set_i_j(downright.i + 1,downright.j + 1);
		downleft.set_i_j(downleft.i + 1,downleft.j - 1);
	}
}

/************************************************************************/
/* best order  2                                                         */
/************************************************************************/
void best_order2(){
	int index,index1;
	int half = matrix_size/2;

	unit_info upleft(half-1 ,half-1) ;
	unit_info upright(half-1 ,half);
	unit_info downleft(half ,half-1);
	unit_info downright(half ,half);

	allocate_unit(upleft.i,upleft.j);
	allocate_unit(upright.i,upright.j);
	allocate_unit(downright.i,downright.j);
	allocate_unit(downleft.i,downleft.j);

	upleft.set_i_j(upleft.i - 1,upleft.j - 1);
	upright.set_i_j(upright.i - 1,upright.j + 1);
	downright.set_i_j(downright.i + 1,downright.j + 1);
	downleft.set_i_j(downleft.i + 1,downleft.j - 1);

	for (index = 0; index < half - 1; index++)
	{
		for (index1 = upleft.j + 2; index1 < upright.j - 1;index1++){			
			allocate_unit(upleft.i,index1);
		}
		for (index1 = upright.i + 2; index1 < downright.i - 1;index1++){
			allocate_unit(index1,upright.j);
		}
		for (index1 = downright.j - 2; index1 > downleft.j + 1;index1--){
			allocate_unit(downleft.i,index1);
		}
		for (index1 = downleft.i - 2; index1 > upleft.i + 1;index1--){
			allocate_unit(index1 , upleft.j);
		}

		allocate_unit(upleft.i ,upleft.j + 1);
		allocate_unit(upright.i,upright.j - 1);
		allocate_unit(upright.i + 1 ,upright.j );
		allocate_unit(downright.i - 1,downright.j);
		allocate_unit(downright.i ,downright.j - 1);
		allocate_unit(downleft.i,downleft.j + 1);
		allocate_unit(downleft.i - 1,downleft.j );
		allocate_unit(upleft.i +1 ,upleft.j );

		allocate_unit(upleft.i,upleft.j);
		allocate_unit(upright.i,upright.j);
		allocate_unit(downright.i,downright.j);
		allocate_unit(downleft.i,downleft.j);
// 		outfile<<upleft.i<<"_"<<upleft.j<<" ";
// 		outfile<<upright.i<<"_"<<upright.j<<" ";
// 		outfile<<downleft.i<<"_"<<downleft.j<<" ";
// 		outfile<<downright.i<<"_"<<downright.j<<" ";
		upleft.set_i_j(upleft.i - 1,upleft.j - 1);
		upright.set_i_j(upright.i - 1,upright.j + 1);
		downright.set_i_j(downright.i + 1,downright.j + 1);
		downleft.set_i_j(downleft.i + 1,downleft.j - 1);
	}
}
void init(){
	int index1,index2;
	for (index1 = 0; index1 <matrix_size;index1 ++)
	{
		for (index2 = 0; index2<matrix_size;index2 ++)
		{
			unit_array[index1][index2].init();
		}
	}
	total_vr = 0;
}
/************************************************************************/
/* �ܷ������                                                           */
/************************************************************************/
void allocate_all(int choice){	
	init();
	total_vr = 0;
	switch (choice)
	{
	case 0:
		normal_order();
		break;
	case 1:
		most_limit_first_order();
		break;
	case 2:
		inward_order();
		break;
	case 3:
		outward_order();
		break;
	case 4:
		best_order();
		break;
	case 5:
		best_order1();
	case 6:
		best_order2();
	case 7:
		best_order3();
	case 8:
		best_order4();
	case 9:
		best_order5();
	case 10:
		best_order6();
	case 11:
		best_order7();
	case 12:
		queue_order();
	}
}
/************************************************************************/
/* ��ʼ��vr������Ϊ���㵥��ΥԼ��׼��                                   */
/************************************************************************/
void set_vr_vector(int x,int y,int kind){
	int index = 0;
	for (index = 0;index < unit_array[x][y].count;index ++)
	{
		vr[unit_array[x][y].content[index]].level[kind-1]++;
	}
}
/************************************************************************/
/* ���㵥��ΥԼ��                                                       */
/************************************************************************/
void calculation_unit_record(int x,int y){
	memset(vr,0,sizeof(violate_record)*(number_scope+2));
	int index1,index2;
	for (index1 = x - 2; index1 <= x + 2; index1++)
	{
		for (index2 = y -2; index2 <= y + 2; index2++)
		{
			if ((index1 >= 0) && (index1 < matrix_size) && (index2 >= 0) && (index2 < matrix_size))//�Ϸ���
			{
				if (abs(index1 - x)==2 || abs(index2 - y)==2)//3��
				{
					set_vr_vector(index1,index2,3);
				}
				else if (abs(index1 - x)==1 || abs(index2 - y)==1)//2��
				{
					set_vr_vector(index1,index2,2);
				}
			}
		}
	}

	int temp_vr = 0;
	for (index1 = 0;index1 < unit_array[x][y].count;index1 ++)
	{
		index2 = unit_array[x][y].content[index1];

		temp_vr += 100 * vr[index2].level[0]; //1����ͬ
		temp_vr += 50 * vr[index2 - 1].level[0];//1��������
		temp_vr += 50 * vr[index2 + 1].level[0];//1��������

		temp_vr += 20 * vr[index2].level[1]; //2����ͬ
		temp_vr += 10 * vr[index2 - 1].level[1];//2��������
		temp_vr += 10 * vr[index2 + 1].level[1];//2��������

		temp_vr += vr[index2].level[2]; //3����ͬ

		vr[index2].level[0]++;
	}

	unit_array[x][y].vr = temp_vr;

	total_vr += temp_vr;

}
/************************************************************************/
/* ��������ΥԼ��                                                       */
/************************************************************************/
void calculation_all(){
	int index1,index2;
	
	for (index1 =0; index1 <matrix_size; index1++)
	{
		for (index2 =0; index2 <matrix_size; index2++)
		{
			calculation_unit_record(index1,index2);
		}
	}
}
/************************************************************************/
/* ��ӡ�������                                                         */
/************************************************************************/
void print_data(){
	int index1,index2,index3;
 		
	for (index1 =0; index1 <matrix_size; index1++)
	{
		outfile<<index1<<"  row  "<<endl;
		for (index2 =0; index2 <matrix_size; index2++)
		{
			outfile<<unit_array[index1][index2].count<<" ";
			for (index3 =0 ; index3 <unit_array[index1][index2].count; index3++)
			{
				outfile<<unit_array[index1][index2].content[index3]<<" ";
			}

			outfile<<unit_array[index1][index2].vr<<endl;
			
		}
		outfile<<endl;
	}

	outfile<<total_vr<<endl;
	cout<<total_vr<<endl;
}

int main(){
	//randint();
// 	int loop = 1;
// 	int choice = 13;
// 	for (int i =0;i<loop;i++)
// 	{
// 		for (int j = 0;j<choice;j++)
// 		{
// 			allocate_all(j);
// 			calculation_all();
// 
// 			cout<<total_vr<<"  ";
// 		}
// 		cout<<endl;
// 	}
	normal_order();
	calculation_all();
	//print_data();

	outfile.close();
	return 1;
}