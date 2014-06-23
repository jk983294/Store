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

    
	//�½�һ����  ���ļ��ж�������
    /*string fName;       //lex�ļ���
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




