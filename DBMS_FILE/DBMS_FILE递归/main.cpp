//#include "btree.h"
#include "StructDefine.h"
#include <iostream>
#include <fstream>
#include <string>
#include "btree.h"
#include <time.h>
using namespace std;

ifstream ifile;

int main()
{
	char *filename="jk.txt";
	char *conffilename = "jk.conf.txt";


	BTree *bt=new BTree(filename,conffilename);
	//bt->createDB();
	bt->openDB();
	//bt->printDBconfig();
	
/*
	time_t start = time(0);
	for (int i=0;i<1000000;i++)
	{
		record *re=new record;
		re->phone_number=i;
		re->pos=i;
		bt->insert_value(*re);
	}
	time_t end = time(0);
	cout<<(end - start)<<endl;
	//bt->printBtree();*/
	//bt->insertRange(1000000);




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



