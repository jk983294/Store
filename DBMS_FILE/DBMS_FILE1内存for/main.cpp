//#include "btree.h"
#include "StructDefine.h"
#include <iostream>
#include <windows.h>
#include <fstream>
#include <string>
#include <ctime>
#include "btree.h"
using namespace std;

ifstream ifile;

int main()
{
	char *filename="jk.txt";
	char *conffilename = "jk.conf.txt";

    
	//新建一棵树  从文件中读入数据
    /*string fName;       //lex文件名
	cout<<"Please enter the name of l file:";
	cin>>fName;
	ifile.open(fName.c_str());*/

	BTree *bt=new BTree(filename,conffilename);
	//bt->createDB();
	bt->openDB();
    //bt->read_block(0);
	//bt->printDBconfig();
	//bt->printDBnode();




	
	time_t start_time; 
	start_time = time(0); 
	cout<<start_time<<endl; 

/*
	for (int i=0;i<1000000;i++)
	{
		record *re=new record;
		re->phone_number=i;
		re->pos=i;
		bt->insert_value(*re);
	}
	*/
	//bt->printBtree();
	
	time_t end_time;

	end_time=time(0);
	cout<<end_time<<endl;

	cout<<(end_time-start_time)<<endl;


	





    cout<<"----------------------------------------------------------"<<endl;
	cout<<"----------------------------------------------------------"<<endl;
	cout<<"------------------号码查询系统-----------------------------"<<endl;
	cout<<"----------------------------------------------------------"<<endl;
	cout<<"----------------------------------------------------------"<<endl;
	cout<<endl<<endl<<endl;
	cout<<"服务的类型"<<endl;
	cout<<"1:按照电话号码寻找位置。"<<endl;
	cout<<"2:添加用户。"<<endl;
	cout<<"3:删除用户。"<<endl;
	cout<<"4:查找一定范围的电话号码。"<<endl;
	cout<<"5:结束"<<endl;
	while (true)
	{
	    cout<<endl<<endl;
		int number;
		int block;
		record *re=new record;
		int nu,pos,nu1;
		cout<<"请输入选择的服务：";
		cin>>number;
		switch (number)
		{
		case 1:
			
			cout<<"请输入您所要查询的电话号码:";
			cin>>nu;
			block=bt->search(nu);
			if (block!=-1)
			{
				//若存在则得到所在的块号
				bt->read_block(block);
				for (int i=0;i<bt->current.count;i++)
				{
					if (nu==bt->current.r[i].phone_number)
					{
						pos=bt->current.r[i].pos;
						
						break;
					}
				}
				cout<<"您所要查询的号码所在的位置是:"<<endl;
				cout<<pos<<endl<<endl;
			}
			else
			{
				cout<<"您所要查询的号码不存在"<<endl;
			}
			break;
		case 2:
			cout<<"请您输入您想添加的号码具体情况。"<<endl;
			cin>>nu>>pos;
			re->phone_number=nu;
			re->pos=pos;
			bt->insert_value(*re);
			break;
		 case 3:
			cout<<"请您输入您想删除的号码具体情况。"<<endl;
			cin>>nu;
			bt->del_value(nu);
			break;
		case 4:
			cout<<"请您输入您想查询的一定范围的号码的区间。"<<endl;
			cin>>nu>>nu1;
			bt->searchScope(nu,nu1);
			break;
		case 5:
			bt->closeDB();
			exit(0);
			break;
		default:
			cout<<"您输入的数字错误，请重新输入"<<endl;
			break;
		}
	}
	

	bt->closeDB();

	return 0;
}




