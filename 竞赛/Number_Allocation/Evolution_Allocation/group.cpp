#include "group.h"

extern ofstream outfile;

violate_record vr[number_scope+2];
int allocat_choice[number_scope+2];	// 0 1 2 .... 70 71  ������������  ���÷�Χ�ж�

int random_scope = matrix_size * matrix_size;

const int crossover_number = 40;
const int mutate_number = 40;
const int evolution_number = 120;






inline bool cmp_unit_info(const unit_info &left, const unit_info &right)
{ 
	return (left.limit > right.limit) ? true : false ; //��С
	//return (left.limit >= right.limit) ? false : true ;//С����
}

inline bool cmp_chromosome(const chromosome &left, const chromosome &right)
{ 
	//return (left.limit > right.limit) ? true : false ; //��С
	return (left.total_vr >= right.total_vr) ? false : true ;//С����
}

void chromosome::init(){
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
//����allocat_choice  Ϊ��Ԫ�����׼��
void chromosome::set_allocat_choice(int x , int y, int kind)
{
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

// 1��   ��ʼ����õ�Ԫ�����  ÿ����һ�����ֵ���һ�Σ�����Ҫѭ��
void chromosome::refresh_allocat_choice(int num)
{
	allocat_choice[num] += 100;
	allocat_choice[num - 1] += 50;
	allocat_choice[num + 1] += 50;
}

//��õ�ǰallocat_choice��С����ѡ��
int chromosome::get_min_allocat_choice()
{
	int index,min_index,min_value;
	min_index = 1;
	min_value = allocat_choice[min_index];
	for (index = 1; index <= number_scope; index++)
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

//����һ����Ԫ�� x,yΪ��������
void chromosome::allocate_unit(int x,int y)
{
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

//��������  ���ϵ��µ�˳����䵥Ԫ��
void chromosome::normal_order()
{
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

void chromosome::best_order()
{
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
//			outfile<<upleft.i<<"_"<<index1<<" ";
		}
		for (index1 = upright.i + 1; index1 < downright.i;index1++){
			allocate_unit(index1,upright.j);
//			outfile<<index1<<"_"<<upright.j<<" ";
		}
		for (index1 = downright.j - 1; index1 > downleft.j;index1--){
			allocate_unit(downleft.i,index1);
//			outfile<<downleft.i<<"_"<<index1<<" ";
		}
		for (index1 = downleft.i - 1; index1 > upleft.i;index1--){
			allocate_unit(index1 , upleft.j);
//			outfile<<index1<<"_"<<upleft.j<<" ";
		}

		allocate_unit(upleft.i,upleft.j);
		allocate_unit(upright.i,upright.j);
		allocate_unit(downleft.i,downleft.j);
		allocate_unit(downright.i,downright.j);

// 		outfile<<upleft.i<<"_"<<upleft.j<<" ";
// 		outfile<<upright.i<<"_"<<upright.j<<" ";
// 		outfile<<downleft.i<<"_"<<downleft.j<<" ";
// 		outfile<<downright.i<<"_"<<downright.j<<" ";

		upleft.set_i_j(upleft.i - 1,upleft.j - 1);
		upright.set_i_j(upright.i - 1,upright.j + 1);
		downleft.set_i_j(downleft.i + 1,downleft.j - 1);
		downright.set_i_j(downright.i + 1,downright.j + 1);

	}
}

void chromosome::random_order()
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

void chromosome::allocate_all(int choice)
{
	switch (choice)
	{
	case 0:
		normal_order();
		break;
	case 1:
		best_order();
		break;
	case 2:
		random_order();
		break;
	}
}

//��ʼ��vr������Ϊ���㵥��ΥԼ��׼��
void chromosome::set_vr_vector(int x,int y,int kind)
{
	int index = 0;
	for (index = 0;index < unit_array[x][y].count;index ++)
	{
		vr[unit_array[x][y].content[index]].level[kind-1]++;
	}
}

//���㵥��ΥԼ��
void chromosome::calculation_unit_violate_record(int x,int y)
{
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

//��������ΥԼ��
void chromosome::calculation_all_unit_violate_record()
{
	total_vr = 0;

	int index1,index2;

	for (index1 =0; index1 <matrix_size; index1++)
	{
		for (index2 =0; index2 <matrix_size; index2++)
		{
			calculation_unit_violate_record(index1,index2);
		}
	}

}

void chromosome::print_data()
{
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

void chromosome::allocate_all_by_sequence(sequence* order){
	int index,index1,index2,seq_value;
	int count = matrix_size*matrix_size;

	for (index = 0; index <count; index ++)
	{
		seq_value = order->seq[index];
		index1 = seq_value /matrix_size;
		index2 = seq_value % matrix_size;
		allocate_unit(index1,index2);	
	}
}

//�������100������Ⱥ
void group::init()
{
	int count = 0;
	chromosome_group.clear();
	while (count < 110)
	{
		chromosome a;
		a.random_order();
		a.best_order();
		a.best_order();
		a.calculation_all_unit_violate_record();

		chromosome_group.push_back(a);

		count++;
	}
	
// 	chromosome temp1;
// 	temp1.best_order();
// 	temp1.best_order();
// 	temp1.calculation_all_unit_violate_record();
// 	chromosome_group.push_back(temp1);
// 
// 	temp1.normal_order();
// 	temp1.calculation_all_unit_violate_record();
// 	chromosome_group.push_back(temp1);


	group_count = chromosome_group.size();
	cout<<"init "<<group_count<<endl;

}
//ѡ��  ��Ӣ
void group::select()
{
	sort(chromosome_group.begin(), chromosome_group.end(), cmp_chromosome);

	int index = 0;
	if (chromosome_group.size() > 100)
	{
		vector<chromosome>::iterator it = chromosome_group.begin();
		for (index = 0; index < 100; index ++ )
		{
			it++;
		}
		
		chromosome_group.erase(it , chromosome_group.end());
	}
	group_count = chromosome_group.size();
	//cout<<"select "<<group_count<<endl;
}
//һ�ν���
void group::crossover()
{
	int index1,index2,random_value1,random_value2;
	random_value1 = rand()%group_count;
	random_value2 = rand()%group_count;

	chromosome temp;
	
	if (random_value1 != random_value2)
	{
		for (index1 =0; index1 <matrix_size; index1++)
		{
			for (index2 =0; index2 <matrix_size; index2++)
			{
				if (rand()%1)
				{
					temp.unit_array[index1][index2] = chromosome_group[random_value1].unit_array[index1][index2];
				}
				else{
					temp.unit_array[index1][index2] = chromosome_group[random_value1].unit_array[index1][index2];
				}
			}

		}

		temp.best_order();
		temp.best_order();
		temp.calculation_all_unit_violate_record();
		cout<<temp.total_vr<<" ";
		chromosome_group.push_back(temp);
	}
	

}
//һ�α���
void group::mutate()
{
	int index1,index2,index3,group_index,random_value1;
	group_index = rand()%group_count;
	int count = 0;

	chromosome temp = chromosome_group[group_index];

	//cout<<
	while(count < random_scope/2){
		index1 = rand()%matrix_size;
		index2 = rand()%matrix_size;

		for (index3 = 0;index3 < temp.unit_array[index1][index2].count;index3 ++ )
		{
			random_value1 = rand()%number_scope +1;
			temp.unit_array[index1][index2].content[index3] = random_value1;
		}

		count++;
		
	}
	temp.best_order();
	temp.best_order();

	temp.calculation_all_unit_violate_record();
	cout<<temp.total_vr<<" ";
	chromosome_group.push_back(temp);
}
//һ�ε���
void group::iteration()
{
	int index1;

	for (index1 = 0; index1 <crossover_number ; index1++)
	{
		crossover();
	}
// 	for (index1 =0; index1 <mutate_number; index1 ++)
// 	{
// 		mutate();
// 	}
	select();
}

void group::evolution()
{
	int index1;

	for (index1 = 0; index1 <evolution_number ; index1++)
	{
		iteration();
		print_all();
	}
}

void group::print_best()
{
	chromosome_group[0].print_data();
}

void group::print_all()
{
	for(vector<chromosome>::iterator it = chromosome_group.begin() ; it != chromosome_group.end() ; it++)  
	{  
		outfile << it->total_vr <<" " ;   
	} 
	outfile<<endl;
}
sequence::sequence(){
	int index;
	for (index = 0; index <matrix_size *matrix_size;index++)
	{
		seq[index] = index;
	}
	count = matrix_size * matrix_size;

	int half = matrix_size/2;
	//center four node
	exchange(0,matrix_size*(half-1)+half - 1);
	exchange(1,matrix_size*(half-1)+half);
	exchange(2,matrix_size*(half)+half);
	exchange(3,matrix_size*(half)+half-1);

}

void sequence::exchange(int i,int j){
	int temp = seq[i];//����a[i] a[i]
	seq[i] = seq[j];
	seq[j] = temp;
}

void sequence::print(int vr , ofstream & order_outfile){
	int i,index1,index2;
	order_outfile<<"vr: "<<vr<<" ";
	for (i = 0; i< count ; i++)
	{
		index1 = seq[i] /matrix_size;
		index2 = seq[i] % matrix_size;
		order_outfile<< index1<<"_"<<index2<<" ";
	}
	order_outfile<<endl<<endl;;
}

/******************************************************************************************************************
������������
******************************************************************************************************************/
void bruteforce_order::perm( const int k, const int n)
{		
	int i;

	if (k == n-1)//��ֹ ��ӡ
	{
		//order.print();
		a.init();
		a.allocate_all_by_sequence(&order);
		a.calculation_all_unit_violate_record();

		//cout<<a.total_vr<<" ";
// 		if (a.total_vr < 164)
// 		{
// 			order.print();
// 		}
		if (a.total_vr < best_vr)
		{
			print_bestseq();
			best_vr = a.total_vr;
		}
		else if (a.total_vr == best_vr)
		{
			print_bestseq();
		}
	} 
	else
	{
		for ( i = k; i<n;i++)
		{
			order.exchange(k,i);

			perm(k+1,n);

			order.exchange(k,i);
		}
	}
					
}

void bruteforce_order::print_bestseq(){
	string filename = ".//out//findbestorder" ;
	char p[32]={0,};
	sprintf(p,"%d",filecount);
	filename = filename + p;
	filename = filename + ".txt";
	//cout<<filename<<" ";

	ofstream best_order_outfile(filename.c_str());

	order.print(a.total_vr,best_order_outfile);

	best_order_outfile.close();

	filecount ++;

}

