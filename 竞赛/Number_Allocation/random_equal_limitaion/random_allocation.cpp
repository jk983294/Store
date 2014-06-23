#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <time.h>

using namespace std;

ofstream outfile("out.txt");


const int number_scope = 70;			// 1 2 ... 70
const int matrix_size = 50;
const int unit_capacity = 5;
// 0: normal_order 1: most_limit_first_order 2: inward_order 3: outward_order 4:best  5 random_order
const int choice = 5;					//

int allocat_choice[number_scope+2];	// 0 1 2 .... 70 71  ������������  ���÷�Χ�ж�

int total_vr = 0;

class unit
{
public:
	unit(){
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
int random_get_min_allocat_choice(){
	int index,min_index,min_value,equal_count;
	min_index = 1;
	equal_count = 1;
	min_value = allocat_choice[min_index];
	for (index = 2; index <= 70; index++)
	{
		if (allocat_choice[index] < min_value )
		{
			min_index = index;
			min_value = allocat_choice[index];
			equal_count = 1;

		}
		if (allocat_choice[index] == min_value)//������Сѡ��
		{
			equal_count++;
		}
	}


	int random_index = rand()%equal_count;
	//cout<<equal_count<<" choices chosen ";
	equal_count = 0;
	for (index = min_index; index <= 70; index++)
	{
		if (allocat_choice[index] == min_value && equal_count == random_index)//������Сѡ��
		{
//			cout<<index<<endl;
			return index;
		}
		else if ( allocat_choice[index] == min_value )
		{
			equal_count ++;
		}
	}

	return min_index;
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
void random_order()
{
	int count = 0;
	int index1,index2,random_value;
	int scope = matrix_size*matrix_size;
	while (count < scope)
	{
		random_value = rand()%scope;
		index1 = random_value /matrix_size;
		index2 = random_value % matrix_size;
		if (unit_array[index1][index2].allocated)
		{
			continue;
		}
		else{
			allocate_unit(index1 , index2);
			count++;
		}
	}
}
/************************************************************************/
/* �ܷ������                                                           */
/************************************************************************/
void allocate_all(){


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
		best_order();
		best_order();
		break;
	case 5:
		random_order();
		best_order();
		best_order();
		best_order();
		break;
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
	srand(time(0));
	allocate_all();
	calculation_all();
	print_data();

	outfile.close();
	return 1;
}