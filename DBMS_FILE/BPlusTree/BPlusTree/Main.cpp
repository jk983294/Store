#include "BPlusTree.h"
#include <iostream>

using namespace std;

int main()
{
	BPlusTree<int, int,10> tree;
	BPlusTreeNode<int,int,10> *nPtr=new BPlusTreeNode<int,int,10>();

	int i;

	for(i=0;i<100000000;i++)   //insert loop
	{
		tree.Add(i,i%5);
	}\
	cout<<"Total numbers of phones:"<<tree.GetKeys()<<endl;
	cout<<"The height of BPlusTree:"<<tree.GetHeight()<<endl;
	//cout<<tree.GetPayload()<<endl;
 
	int command=0;

    cout<<"If search single phone number,input 1."<<endl;
	cout<<"If search a range of phone numbers,input 2."<<endl;
	cout<<"If delete a phone number, input 3."<<endl;
	cout<<"If exit,input -1."<<endl;

	while (command!=-1)
	{
		cout<<"Please input command number: ";
		cin>>command;
		switch(command)
		{
		case 1:
			tree.SingleSearch();
			break;
		case 2:
			tree.RangeSearchApp();		
			break;
		case 3:
			tree.DeleteApp();
			break;	
		case 4:
			tree.OutputBPTree();
			break;
		}
	}

	return 0;

}
