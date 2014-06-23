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
	cout<<"------------------�����ѯϵͳ-----------------------------"<<endl;
	cout<<"----------------------------------------------------------"<<endl;
	cout<<"----------------------------------------------------------"<<endl;
	cout<<endl<<endl<<endl;
	cout<<"���������"<<endl;
	cout<<"1:���յ绰����Ѱ��λ�á�"<<endl;
	cout<<"2:����û���"<<endl;
	cout<<"3:ɾ���û���"<<endl;
	cout<<"4:����һ����Χ�ĵ绰���롣"<<endl;
	cout<<"5:����"<<endl;
	while (true)
	{
	    cout<<endl<<endl;
		int number;
		int block;
		record *re=new record;
		int nu,pos,nu1;
		cout<<"������ѡ��ķ���";
		cin>>number;
		switch (number)
		{
		case 1:
			
			cout<<"����������Ҫ��ѯ�ĵ绰����:";
			cin>>nu;
			block=bt->search(nu);
			if (block!=-1)
			{
				//��������õ����ڵĿ��
				bt->read_block(block);
				for (int i=0;i<bt->current.count;i++)
				{
					if (nu==bt->current.r[i].phone_number)
					{
						pos=bt->current.r[i].pos;
						
						break;
					}
				}
				cout<<"����Ҫ��ѯ�ĺ������ڵ�λ����:"<<endl;
				cout<<pos<<endl<<endl;
			}
			else
			{
				cout<<"����Ҫ��ѯ�ĺ��벻����"<<endl;
			}
			break;
		case 2:
			cout<<"��������������ӵĺ�����������"<<endl;
			cin>>nu>>pos;
			re->phone_number=nu;
			re->pos=pos;
			bt->insert_value(*re);
			break;
		case 3:
			cout<<"������������ɾ���ĺ�����������"<<endl;
			cin>>nu;
			bt->del_value(nu);
			break;
		case 4:
			cout<<"�������������ѯ��һ����Χ�ĺ�������䡣"<<endl;
			cin>>nu>>nu1;
			bt->searchScope(nu,nu1);
			break;
		case 5:
			bt->closeDB();
			exit(0);
			break;
		default:
			cout<<"����������ִ�������������"<<endl;
			break;
		}
	}
	

	bt->closeDB();

	return 0;
}



