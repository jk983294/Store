#pragma warning(disable:4786)
#include <iostream>
#include <set>
#include <queue>
#include <windows.h>
#include "btree.h"
using namespace std; 

class setcmp:greater<record>  
{  
public:  
    bool     operator () (record b1,record b2) const{  
        return b1.phone_number<b2.phone_number;  
	}  
};

void BTree::createDB()
{
	file.open(filename,ios::out|ios::binary|ios::in);
	//conf_file.open(conffilename,ios::out|ios::binary|ios::in);
	if(!file.good()){
		cout<<"file error"<<endl;
	}
	conf.count = 0;
	conf.start_block=-1;
}

void BTree::openDB()//打开配置文件
{
	file.open(filename,ios::out|ios::binary|ios::in);
	int num,idle;
	conf_file.open(conffilename,ios::out|ios::binary|ios::in);
	conf_file>>conf.count>>conf.start_block>>num;
	for (int i=0;i<num;i++)
	{
		conf_file>>idle;
		idle_block.push_back(idle);
	}
	conf_file.close();
}

void BTree::closeDB()
{
	conf_file.open(conffilename,ios::out|ios::in);
	conf_file<<conf.count<<" "<<conf.start_block<<" "<<idle_block.size()<<" ";
	for (int i=0;i<idle_block.size();i++)
	{
		conf_file<<idle_block[i]<<" ";
	}
	conf_file.close();
	//cout<<"conf.count:"<<conf.count<<endl;

	if (file.is_open())
	{
		file.close();
	}
}

void BTree::printDBconfig()
{
	cout<<"block:"<<conf.count<<"  idle block:"<<idle_block.size()<<"   start_block:  "<<conf.start_block<<endl;
	for (int i =0;i<idle_block.size();i++)
	{
		cout<<" "<<idle_block[i];
	}
	cout<<endl;
}

void BTree::printDBnode()
{
	cout<<"data count"<<current.count<<" data:";
	for (int i = 0;i<current.count;i++)
	{
		cout<<current.r[i].phone_number<<" ";
	}
	cout<<" pointer:";
	for (i = 0;i<current.count+1;i++)
	{
		cout<<current.block_ptr[i]<<" ";
	}
	cout<<endl;
	
}

void BTree::read_block(int i)//读记录块
{

	file.seekg(i*block_size);

	file.read(reinterpret_cast<char *>(&current),block_size);

}
void BTree::write_block(int i)//写记录块
{
	file.seekp(ios::beg);// 写指针回到文件首部	
	file.seekp(i*block_size,ios::beg);//移动到第几个记录

	
	file.write(reinterpret_cast<char *>(&current),block_size);
}


void BTree::initNode(){
	current.count=0;
	for (int i=0;i<2*M+1;i++)
	{
		current.block_ptr[i]=-1;
	}
}

int BTree::search(int number)                //查找指定结点,返回结点是否存在  返回-1表示不存在该节点中
{

    //根据配置文件查找根节点所在的记录块
	//读取记录块到current结点中 
	int count=conf.count; //读取配置信息里的块总数
    int start=conf.start_block;  //从配置文件中读出根节点所在的块号
	if (count==0 && start==-1)  //一开始没有根节点
	{
		return -1;       //表示没有数据
	}
	else   //否则不为空
	{
		do 
		{
			read_block(start);
			for (int i=0;i<current.count;i++)
			{
				if (current.r[i].phone_number==number)
				{
					return start;
				}
				else if (current.r[i].phone_number>number)
				{
					start=current.block_ptr[i];
					break;
				}
				else
				{
					if (i==current.count-1)
					{
						start=current.block_ptr[current.count];
					}
					else
						continue;
				}
			}
			
		} while (start!=-1);
	   return -1;
		
	

	}
	
         
}

void BTree::searchScope(int a,int b)  //查找在a-b之间的电话号码
{
	//vector<record> find;
	set<record,setcmp> find;
	while (!path.empty())
	{
		path.pop();
	}
	int read=conf.start_block;//起始节点
	int count=0;//计数  找到几个
	path.push(read);
	while (path.size()!=0)
	{
		read = path.top();
		path.pop();
		read_block(read);
		for (int i=0;i<current.count;i++)//每个记录只考虑左指针   最后一个记录考虑右指针
		{
			if ((i==current.count-1) && (current.r[i].phone_number< b)
				&&(current.block_ptr[i+1] != -1))//扫到该结点最后一个记录  考虑右指针 
			{
				path.push(current.block_ptr[i+1]);
			}
			if (current.r[i].phone_number>a )
			{
				if (current.r[i].phone_number<=b)
				{
					find.insert(current.r[i]);
				}				
				if (current.block_ptr[i] != -1)//考虑左指针
				{
					path.push(current.block_ptr[i]);
				}
				if (current.r[i].phone_number>b)//找到一个结点中第一个大于范围的记录  跳出
				{
					break;
				}
			}
			if (current.r[i].phone_number==a)
			{
				find.insert(current.r[i]);
			}
			else if (current.r[i].phone_number==b)
			{
				break;	
			}
		}
	}
	cout<<"find "<<find.size()<<" record"<<endl;
	for(set<record,setcmp>::iterator iter=find.begin();iter!=find.end();iter++){
		printrecord(*iter);
	}
	
}

void BTree::printrecord(record a){
	cout<<"phone number: "<<a.phone_number<<"  "<<"phone area: "<<a.pos<<endl;
}

void BTree::printBtree(){
	queue<int> order;
	int read;
	order.push(conf.start_block);
	while(order.size()!=0){
		read = order.front();
		order.pop();
		read_block(read);
		printDBnode();
		for(int i=0;i<=current.count;i++){
			if (current.block_ptr[i] != -1)
			{
				order.push(current.block_ptr[i]);
			}
			
		}
	}
}

bool BTree::get_path(int nu)     //在插入的时候要用到
{
	//cout<<path.size();
	while (!path.empty())
	{
		path.pop();
	}
	
	int count=conf.count; //读取配置信息里的块总数
    int start=conf.start_block;  //从配置文件中读出根节点所在的块号
	if (count==0 && start==-1)  //一开始没有根节点
	{
		//开一块空间
		initNode();
		conf.start_block=0; //重新赋值
		conf.count=1;       //重新赋值
		write_block(0);   //写0块
		path.push(0);
		return false;
	}
	else{
		do 
		{
			path.push(start);
			read_block(start);
			for (int i=0;i<current.count;i++)
			{
				if (current.r[i].phone_number==nu)
				{
					return true;
				}
				else if (current.r[i].phone_number>nu)
				{
					start=current.block_ptr[i];
					break;
				}
				else
				{
					if (i==current.count-1)
					{
						start=current.block_ptr[current.count];
					}
					else
						continue;
				}
			}
			
		} while (start!=-1);
		
		return false;     
		
	}
	
	
	
}

void BTree::insert_value(record re)     //添加记录
{
	//添加记录  首先查找是否存在该记录  并将路径记录在path栈中
	bool isExist=get_path(re.phone_number);
	if (isExist)
	{
		cout<<"该节点已存在"<<endl;
		return; 
	}
	else{    //该节点不存在则插入节点且路径记录在path中
        int number=path.top();//获取最后一个结点所在的块号
		path.pop();   
		read_block(number);  //读取信息
		if (current.count==2*M)  //若结点已满
		{
			//分裂结点
			split_node(re,number,-1,-1);  
		   
			
			
		}
		else
		{
			//通过比较插入
			if (current.count==0)
			{
				//直接插入
				current.r[0]=re;

			}
			int pos=0;
            for (int i=0;i<current.count;i++)
            {
                if (current.r[i].phone_number<re.phone_number)
                {
					//pos++;
					//pos = i+1;
					pos++;
					continue;
                }
				else
				{
					pos=i;
					break;
				}
            }
            for (int j=current.count;j>=(pos+1);j--)
            {
				current.r[j]=current.r[j-1];
				
            } //移动位置
			current.r[pos]=re;
			current.count++;
			write_block(number);  //写回块的信息
		}
		
	}
   

}


void BTree::split_node(record re,int block,int left,int right)     //分裂结点
{
	int root;
	//cout<<path.size()<<endl;

    read_block(block);  //读取块信息
	if (current.count==2*M)  //若结点已满
	{
		//分裂结点
		//确定要加入的位置
		int pos=0;
		for (int i=0;i<current.count;i++)
		{
			if (current.r[i].phone_number<re.phone_number)
			{
				pos++;
				continue;
			}
			else
			{
				pos=i;
				break;
			}
		}
		
		if (pos==M)   //若插入的位置在中间则将插入的结点的值作为父亲结点
		{
			//处理两个结点  修改block结点内容  添加新的结点
			//node *newnode=new node;
			newnode=current;
			//newnode=&current;  //
			current.count=M;
			current.block_ptr[M]=left;
			write_block(block);  //协会磁盘
            conf.count++;   //块数加1
			initNode(); //重新生成一个结点
            current.count=M;
			//开始赋值
			for (i=0;i<M;i++)
			{
				current.r[i].phone_number=newnode.r[i+M].phone_number;
				current.r[i].pos=newnode.r[i+M].pos;
				current.block_ptr[i]=newnode.block_ptr[i+M];

			}
			current.block_ptr[0]=right;
			current.block_ptr[M]=newnode.block_ptr[2*M];
			write_block(conf.count-1);
			if (path.size()==0)
			{
				initNode();
				current.count=1;
				conf.count++;
				current.r[0]=re;
				current.block_ptr[0]=block;
				current.block_ptr[1]=conf.count-2;
				conf.start_block=conf.count-1;
				write_block(conf.count-1);
			}
			else
			{
				root =path.top();  //得到下一个根结点
		        path.pop();
                split_node(re,root,block,conf.count-1);    
			}
		}
		else if (pos<M)   //则m-1位置上的记录作为父亲结点
		{
			record *parent=new record;
			*parent=current.r[M-1];
			//node *newnode=new node;
			newnode=current;  //
			current.count=M;
            for (i=M-2;i>=pos;i--)
            {
                current.r[i+1]=current.r[i];
				current.block_ptr[i+2]=current.block_ptr[i+1];

            }
			current.r[pos]=re;
            //调整指针
            current.block_ptr[pos]=left;
			current.block_ptr[pos+1]=right;

			write_block(block);  //协会磁盘
            conf.count++;
			initNode(); //重新生成一个结点
            current.count=M;
			//开始赋值
			for (i=0;i<M;i++)
			{
				current.r[i].phone_number=newnode.r[i+M].phone_number;
				current.r[i].pos=newnode.r[i+M].pos;
				current.block_ptr[i]=newnode.block_ptr[i+M];
				
			}
			current.block_ptr[M]=newnode.block_ptr[2*M];
			write_block(conf.count-1);
			if (path.empty())
			{
				initNode();
				current.count=1;
				conf.count++;
				current.r[0]=*parent;
				current.block_ptr[0]=block;
				current.block_ptr[1]=conf.count-2;
				conf.start_block=conf.count-1;
				write_block(conf.count-1);
			}
			else
			{
				root =path.top();  //得到下一个根结点
				path.pop();
                split_node(*parent,root,block,conf.count-1);       
			}
			
			
			

		}
		else if (pos>M)   //则M位置上的记录作为父亲结点
		{
			record *parent=new record;
			*parent=current.r[M];
			//node *newnode=new node;
			newnode=current;  //
			current.count=M;
			//
			write_block(block);  //协会磁盘
            conf.count++;
			initNode(); //重新生成一个结点
            current.count=M;
			//开始赋值
			for (i=M+1;i<pos;i++)
			{
				current.r[i-M-1].phone_number=newnode.r[i].phone_number;
				current.r[i-M-1].pos=newnode.r[i].pos;
				current.block_ptr[i-M-1]=newnode.block_ptr[i];
				
			}
			current.r[pos-M-1]=re;
			for (i=pos-M;i<M;i++)
			{
				current.r[i].phone_number=newnode.r[i+M].phone_number;
				current.r[i].pos=newnode.r[i+M].pos;
				current.block_ptr[i+1]=newnode.block_ptr[i+M+1];
			}
			current.block_ptr[pos-M-1]=left;
			current.block_ptr[pos-M]=right;
			//current.block_ptr[M]=newnode->block_ptr[M];
			write_block(conf.count-1);

			if (path.empty())
			{
				initNode();
				current.count=1;
				conf.count++;
				current.r[0]=*parent;
				current.block_ptr[0]=block;
				current.block_ptr[1]=conf.count-2;
				conf.start_block=conf.count-1;
				write_block(conf.count-1);
			}
			else
			{
				root =path.top();  //得到下一个根结点
				path.pop();
                split_node(*parent,root,block,conf.count-1);       
			}
			
		}

		
		} 
		else
		{
			int pos=0;
            for (int i=0;i<current.count;i++)
            {
                if (current.r[i].phone_number<re.phone_number)
                {
					//pos++;
					pos++;
					continue;
                }
				else
				{
					pos=i;
					break;
				}
            }

			current.block_ptr[current.count+1]=current.block_ptr[current.count];
            for (int j=current.count;j>=(pos+1);j--)
            {
				current.r[j]=current.r[j-1];
				current.block_ptr[j]=current.block_ptr[j-1];
				
            } //移动位置

			current.count++;
			
			current.r[pos]=re;
			current.block_ptr[pos]=left;
			current.block_ptr[pos+1]=right;
			
			write_block(block);  //写回块的信息

		}
}
void BTree::insertRange(int range){
	Sleep(1000*60*1);
}
int BTree::get_path1(int number)   //返回该数据在那块中  并将路径记录下来
{
	while (!path.empty())
	{
		path.pop();
	}
	
	int count=conf.count; //读取配置信息里的块总数
    int start=conf.start_block;  //从配置文件中读出根节点所在的块号
	if (count==0 && start==-1)  //一开始没有根节点
	{
		return -1;   //不存在
	}
	else{
		do 
		{
			path.push(start);
			read_block(start);
			for (int i=0;i<current.count;i++)
			{
				if (current.r[i].phone_number==number)
				{
					return start; 
				}
				else if (current.r[i].phone_number>number)
				{
					start=current.block_ptr[i];
					break;
				}
				else
				{
					if (i==current.count-1)
					{
						start=current.block_ptr[current.count];
					}
					else
						continue;
				}
			}
			
		} while (start!=-1);
		
		return -1;     
		
	}
	

}
bool BTree::isLeaf(int block){
	read_block(block);
	for (int i=0;i<=current.count;i++)
	{
		if (current.block_ptr[i]==-1)
		{
			return  true;
		}
	}
	return false;
}
void BTree::del_value(int nu)        //删除记录
{
	int root,pos;
	int i,j;
	int parent_block,left_brother_block,right_brother_block,deletenode_block;
	int left_parent_pos,right_parent_pos;

	//删除记录  首先查找是否存在该记录  并将路径记录在path栈中
	int isExist=get_path1(nu);
	if (isExist==-1)
	{
		cout<<"该节点不存在"<<endl;
		return; 
	}
	else{    //该节点存在
		//判断该结点是否是叶节点；
        bool leaf=isLeaf(isExist);
        if (leaf)   //是叶子结点
        {
            int number=path.top();//要删除的块号
			path.pop();   
		    read_block(number);  //读取信息
			deletenode=current;
			deletenode_block=number;
			if (conf.start_block==number)   //只有根节点
			{
				//直接删除
				//直接删除
				pos=0;
				for (i=0;i<current.count;i++)
				{
					if (current.r[i].phone_number==nu)
					{
						pos=i;
						break; 
					}
					
				}
				for (j=pos+1;j<current.count;j++)
				{
					current.r[j-1]=current.r[j];
					
				} //移动位置
				current.count--;
				write_block(number);  //写回块的信息

			}
			else        //非根结点
			{
				if (current.count>M)
				{
					//直接删除
					pos=0;
					for (i=0;i<current.count;i++)
					{
						if (current.r[i].phone_number==nu)
						{
							pos=i;
							break; 
						}
						
					}
					for (j=pos+1;j<current.count;j++)
					{
						current.r[j-1]=current.r[j];
						
					} //移动位置
					current.count--;
					write_block(number);  //写回块的信息
					
				}
				else 
				{
					//不直接删除  找兄弟结点
					//找父亲结点
					root=path.top();
					path.pop();
					read_block(root);
					parent=current;  //父亲结点
					parent_block=root;
					//找兄弟结点  先判定自己在那个位置
					for (i=0;i<=current.count;i++)
					{
						if (current.block_ptr[i]==number)
						{
							pos=i;
							break;
						}
                    
					}
                    //pos是指针的位置
					if (pos==0)    //自己是第一个孩子  没有左兄弟
					{
						right_parent_pos=pos;
						read_block(parent.block_ptr[pos+1]);   //读出右兄弟
						right_brother=current;
						right_brother_block=parent.block_ptr[pos+1];
						if (current.count>M)
						{
							//给它一个孩子  
							//父亲结点给它自己 
							//先将那个数据删除  
							for (i=0;i<deletenode.count;i++)
							{
								if (deletenode.r[i].phone_number==nu)
								{
									pos=i;
									break; 
								}
								
							}
							for (j=pos+1;j<deletenode.count;j++)
							{
								deletenode.r[j-1]=deletenode.r[j];
								
							} //移动位置
							//加入父亲结点
							deletenode.r[deletenode.count-1]=parent.r[right_parent_pos];

							parent.r[right_parent_pos]=right_brother.r[0];

							//右兄弟中删除结点0
							right_brother.count--;
							for (i=0;i<right_brother.count;i++)
							{
								right_brother.r[i]=right_brother.r[i+1];
							}
						

							current=parent;
							write_block(parent_block);

							current=deletenode;
							write_block(deletenode_block);

							current=right_brother;
							write_block(right_brother_block);


			             }
						else
						{    //没有多余的孩子  只能合并  
							//先将那个记录删除
							for (i=0;i<deletenode.count;i++)
							{
								if (deletenode.r[i].phone_number==nu)
								{
									pos=i;
									break; 
								}
								
							}
							for (j=pos+1;j<deletenode.count;j++)
							{
								deletenode.r[j-1]=deletenode.r[j];
								
							} //移动位置
							deletenode.count--;

							combine_node(deletenode,parent,right_brother,deletenode_block,parent_block,right_brother_block,right_parent_pos);
							




						}

					}
					else if (pos==parent.count)
					{
						//最后一个儿子
						left_parent_pos=pos-1;
						read_block(current.block_ptr[pos-1]);   //读出右兄弟
						left_brother=current;
						left_brother_block=parent.block_ptr[pos-1];
						if (current.count>M)
						{
							//给它一个孩子  
							//父亲结点给它自己 
							//先将那个数据删除  
							for (i=0;i<deletenode.count;i++)
							{
								if (deletenode.r[i].phone_number==nu)
								{
									pos=i;
									break; 
								}
								
							}
							for (j=pos+1;j<deletenode.count;j++)
							{
								deletenode.r[j-1]=deletenode.r[j];
								
							} //移动位置
							//加入父亲结点
							for (i=deletenode.count;i>=1;i--)
							{
								deletenode.r[i]=deletenode.r[i-1];
							}
							deletenode.r[0]=parent.r[left_parent_pos];
							
							parent.r[left_parent_pos]=left_brother.r[left_brother.count-1];
							
							//右兄弟中删除结点0
							left_brother.count--;
							
							
							current=parent;
							write_block(parent_block);
							
							current=deletenode;
							write_block(deletenode_block);
							
							current=left_brother;
							write_block(left_brother_block);
							
							
						}
						else
						{    //没有多余的孩子  只能合并
							for (i=0;i<deletenode.count;i++)
							{
								if (deletenode.r[i].phone_number==nu)
								{
									pos=i;
									break; 
								}
								
							}
							for (j=pos+1;j<deletenode.count;j++)
							{
								deletenode.r[j-1]=deletenode.r[j];
								
							} //移动位置
							deletenode.count--;
							
							combine_node(left_brother,parent,deletenode,left_brother_block,parent_block, deletenode_block,left_parent_pos);
							
							
						}


					}
					else          //既有左孩子 又有右孩子
					{
						//先读出左孩子
						left_parent_pos=pos-1;
						read_block(current.block_ptr[pos-1]);   //读出左兄弟
						left_brother=current;
						left_brother_block=parent.block_ptr[pos-1];
						if (current.count>M)
						{
							//给它一个孩子  
							//父亲结点给它自己 
							//先将那个数据删除  
							for (i=0;i<deletenode.count;i++)
							{
								if (deletenode.r[i].phone_number==nu)
								{
									pos=i;
									break; 
								}
								
							}
							for (j=pos+1;j<deletenode.count;j++)
							{
								deletenode.r[j-1]=deletenode.r[j];
								
							} //移动位置
							//加入父亲结点
							for (i=deletenode.count;i>=1;i--)
							{
								deletenode.r[i]=deletenode.r[i-1];
							}
							deletenode.r[0]=parent.r[left_parent_pos];
							
							parent.r[left_parent_pos]=left_brother.r[left_brother.count-1];
							
							//右兄弟中删除结点0
							left_brother.count--;
							
							
							current=parent;
							write_block(parent_block);
							
							current=deletenode;
							write_block(deletenode_block);
							
							current=left_brother;
							write_block(left_brother_block);
							
							
						}
						else
						{ 
							//没有足够分给兄弟
							right_parent_pos=pos;
							read_block(parent.block_ptr[pos+1]);   //读出右兄弟
							right_brother=current;
							right_brother_block=parent.block_ptr[pos+1];
							if (current.count>M)
							{
								//给它一个孩子  
								//父亲结点给它自己 
								//先将那个数据删除  
								for (i=0;i<deletenode.count;i++)
								{
									if (deletenode.r[i].phone_number==nu)
									{
										pos=i;
										break; 
									}
									
								}
								for (j=pos+1;j<deletenode.count;j++)
								{
									deletenode.r[j-1]=deletenode.r[j];
									
								} //移动位置
								//加入父亲结点
								deletenode.r[deletenode.count-1]=parent.r[right_parent_pos];
								
								parent.r[right_parent_pos]=right_brother.r[0];
								
								//右兄弟中删除结点0
								right_brother.count--;
								for (i=0;i<right_brother.count;i++)
								{
									right_brother.r[i]=right_brother.r[i+1];
								}
								
								
								current=parent;
								write_block(parent_block);
								
								current=deletenode;
								write_block(deletenode_block);
								
								current=right_brother;
							    write_block(right_brother_block);
								
								
							}
							else
							{
								//合并  默认和左兄弟合并
								for (i=0;i<deletenode.count;i++)
								{
									if (deletenode.r[i].phone_number==nu)
									{
										pos=i;
										break; 
									}
									
								}
								for (j=pos+1;j<deletenode.count;j++)
								{
									deletenode.r[j-1]=deletenode.r[j];
									
								} //移动位置
								deletenode.count--;
								
							combine_node(left_brother,parent,deletenode,left_brother_block,parent_block, deletenode_block,left_parent_pos);

							}
						}

					}
                    
					
					
					
				}

			}
		
        }
		else        //非叶子结点
		{
			//用右子树的的最小元素代替被删除的元素
			//获得有子树的最小元素
			int number =path.top();    //获得中间结点的编号
			path.pop();
			//定位自己
			read_block(number);
            for (i=0;i<current.count;i++)
            {
                if (current.r[i].phone_number==nu)
                {
					pos=i;
					break;
                }
            }
			read_block(current.block_ptr[pos+1]);
			while(current.block_ptr[0]!=-1){
                read_block(current.block_ptr[0]);
			}
			//获得记录
			record *re=new record;
			*re=current.r[0];
			int phonenumber=current.r[0].phone_number;

			

			del_value(phonenumber);

			//查找要删除的记录
			int  block=search(nu);
			read_block(block);
			for (i=0;i<current.count;i++)
			{
				if (current.r[i].phone_number==nu)
				{
					current.r[i]=*re;
				}
			}
			write_block(block);

	}
       
	}
	
}

void BTree::combine_node(node left,node parent,node right,int left_block,int parent_block,int right_block,int parent_pos)   //合并结点
{
	int root,pos;
	int i,j;
	int parent_block1,left_brother_block,right_brother_block,deletenode_block;
	int left_parent_pos,right_parent_pos;

    if (parent_block==conf.start_block)   //父亲是根节点
    {
		if (parent.count==1)
		{
			//直接合并
			left.count++;
	        left.r[left.count-1]=parent.r[parent_pos];//将父亲结点合并

			for (i=0;i<parent.count;i++)
			{
				parent.r[i].phone_number=-100;
				parent.r[i].pos=-100;
				parent.block_ptr[i]=-1;
			}
			parent.block_ptr[parent.count]=-1;
			current=parent;
			write_block(parent_block);
            idle_block.push_back(parent_block);



			//右兄弟
			
			for (i=0;i<right.count;i++)
			{
				left.count++;
				left.r[left.count-1]=right.r[i];
				left.block_ptr[left.count-1]=right.block_ptr[i];
			}
			
			left.block_ptr[left.count]=right.block_ptr[right.count];
			for (i=0;i<right.count;i++)
			{
				right.r[i].phone_number=-100;
				right.r[i].pos=-100;
				right.block_ptr[i]=-1;
			}
			right.block_ptr[right.count]=-1;
            current=right;
			write_block(right_block);
            idle_block.push_back(right_block);


			current=left;
			write_block(left_block);

		}
		else
		{
			left.count++;
			left.r[left.count-1]=parent.r[parent_pos];//将父亲结点合并
			
			//父亲结点被改变
			for (i=parent_pos;i<parent.count;i++)
			{
				parent.r[i]=parent.r[i+1];
				
				
			}
			for (i=parent_pos+1;i<parent.count;i++)
			{
				parent.block_ptr[i]=parent.block_ptr[i+1];
			}
			parent.count--;
			current=parent;

			write_block(parent_block);
			
			//右兄弟
			
			for (i=0;i<right.count;i++)
			{
				left.count++;
				left.r[left.count-1]=right.r[i];
				left.block_ptr[left.count-1]=right.block_ptr[i];
			}
			
			left.block_ptr[left.count]=right.block_ptr[right.count];
			for (i=0;i<right.count;i++)
			{
				right.r[i].phone_number=-100;
				right.r[i].pos=-100;
				right.block_ptr[i]=-1;
			}
			right.block_ptr[right.count]=-1;
			current=right;
			write_block(right_block);
            idle_block.push_back(right_block);


			current=left;
			write_block(left_block);

		}
    }
	else    //不是根结点
	{
		//若不是根结点的话  至少有M个结点
		//先合并
		
		
		left.count++;
		left.r[left.count-1]=parent.r[parent_pos];//将父亲结点合并
		
		//父亲结点被改变
		for (i=parent_pos;i<parent.count;i++)
		{
			parent.r[i]=parent.r[i+1];
			
			
		}
		for (i=parent_pos+1;i<parent.count;i++)
		{
			parent.block_ptr[i]=parent.block_ptr[i+1];
		}
		parent.count--;
		current=parent;
		write_block(parent_block);
		
		//右兄弟
		
		for (i=0;i<right.count;i++)
		{
			left.count++;
			left.r[left.count-1]=right.r[i];
			left.block_ptr[left.count-1]=right.block_ptr[i];
		}
		
		left.block_ptr[left.count]=right.block_ptr[right.count];
		for (i=0;i<right.count;i++)
		{
			right.r[i].phone_number=-100;
			right.r[i].pos=-100;
			right.block_ptr[i]=-1;
		}
		right.block_ptr[right.count]=-1;
		current=right;
		write_block(right_block);
        idle_block.push_back(right_block);

		current=left;
		write_block(left_block);


		if (parent.count>=M)
		{
			//结束
			return;
		}
		else
		{
			//进行再合并
			//若是跟结点  完成
			deletenode=parent;
			deletenode_block=parent_block;

			root=path.top();
			path.pop();
			read_block(root);  
			parent=current;

			//定位
			parent_block=root;  //
			//找兄弟结点  先判定自己在那个位置
			for (i=0;i<=parent.count;i++)
			{
				if (current.block_ptr[i]==deletenode_block)
				{
					pos=i;
					break;
				}
				
			}
			if (pos==0)
			{
				right_parent_pos=pos;
				read_block(parent.block_ptr[pos+1]);   //读出右兄弟
				right_brother=current;
				right_brother_block=parent.block_ptr[pos+1];
				if (current.count>M)
				{
					//给它一个孩子  
					//父亲结点给它自己 
					//加入父亲结点
					deletenode.count++;
					deletenode.r[deletenode.count-1]=parent.r[right_parent_pos];
					
					parent.r[right_parent_pos]=right_brother.r[0];

					deletenode.block_ptr[deletenode.count]=right_brother.block_ptr[0];
					
					//右兄弟中删除结点0
					right_brother.count--;
					for (i=0;i<right_brother.count;i++)
					{
						right_brother.r[i]=right_brother.r[i+1];
					}
					
					
					current=parent;
					write_block(parent_block);
					
					current=deletenode;
					write_block(deletenode_block);
					
					current=right_brother;
					write_block(right_brother_block);
					
					
			    }
				else
				{    //没有多余的孩子  只能合并  
					//先将那个记录删除
					
					combine_node(deletenode,parent,right_brother,deletenode_block,parent_block,right_brother_block,right_parent_pos);
					
					
					
					
					
				}

			}
			else if (pos==parent.count)
			{
				//最后一个儿子
				left_parent_pos=pos-1;
				read_block(current.block_ptr[pos-1]);   //读出左兄弟
				left_brother=current;
				left_brother_block=parent.block_ptr[pos-1];
				if (current.count>M)
				{
					//给它一个孩子  
					//父亲结点给它自己 
					//加入父亲结点
					deletenode.block_ptr[deletenode.count+1]=deletenode.block_ptr[deletenode.count];
					for (i=deletenode.count;i>=1;i--)
					{
						deletenode.r[i]=deletenode.r[i-1];
						deletenode.block_ptr[i]=deletenode.block_ptr[i-1];
					}
					deletenode.count++;
					//deletenode.block_ptr[deletenode.count]=deletenode.block_ptr[deletenode.count-1];
					deletenode.r[0]=parent.r[left_parent_pos];
					deletenode.block_ptr[0]=left_brother.block_ptr[left_brother.count];
					
					parent.r[left_parent_pos]=left_brother.r[left_brother.count-1];
					
					//右兄弟中删除结点0
					left_brother.count--;
					
					
					current=parent;
					write_block(parent_block);
					
					current=deletenode;
					write_block(deletenode_block);
					
					current=left_brother;
					write_block(left_brother_block);
					
					
				}
				else
				{    //没有多余的孩子  只能合并
			
					
					combine_node(left_brother,parent,deletenode,left_brother_block,parent_block, deletenode_block,left_parent_pos);
					
					
				}

			}
			else
			{
				//先读出左孩子
				left_parent_pos=pos-1;
				read_block(current.block_ptr[pos-1]);   //读出左兄弟
				left_brother=current;
				left_brother_block=parent.block_ptr[pos-1];
				if (current.count>M)
				{
					//给它一个孩子  
					//父亲结点给它自己 
					//加入父亲结点
					deletenode.block_ptr[deletenode.count+1]=deletenode.block_ptr[deletenode.count];
					for (i=deletenode.count;i>=1;i--)
					{
						deletenode.r[i]=deletenode.r[i-1];
						deletenode.block_ptr[i]=deletenode.block_ptr[i-1];
					}
					deletenode.count++;
					//deletenode.block_ptr[deletenode.count]=deletenode.block_ptr[deletenode.count-1];
					deletenode.r[0]=parent.r[left_parent_pos];
					deletenode.block_ptr[0]=left_brother.block_ptr[left_brother.count];
					
					parent.r[left_parent_pos]=left_brother.r[left_brother.count-1];
					
					//右兄弟中删除结点0
					left_brother.count--;
					
					
					current=parent;
					write_block(parent_block);
					
					current=deletenode;
					write_block(deletenode_block);
					
					current=left_brother;
					write_block(left_brother_block);
					
					
				}
							
				else
						{ 
							//没有足够分给兄弟
							//加入父亲结点
							right_parent_pos=pos;
							read_block(parent.block_ptr[pos+1]);   //读出右兄弟
							right_brother=current;
							right_brother_block=parent.block_ptr[pos+1];
							if (current.count>M)
							{
								//给它一个孩子  
								//父亲结点给它自己 
								//加入父亲结点
								deletenode.count++;
								deletenode.r[deletenode.count-1]=parent.r[right_parent_pos];
								
								parent.r[right_parent_pos]=right_brother.r[0];
								
								deletenode.block_ptr[deletenode.count]=right_brother.block_ptr[0];
								
								//右兄弟中删除结点0
								right_brother.count--;
								for (i=0;i<right_brother.count;i++)
								{
									right_brother.r[i]=right_brother.r[i+1];
								}
								
								
								current=parent;
								write_block(parent_block);
								
								current=deletenode;
								write_block(deletenode_block);
								
								current=right_brother;
								write_block(right_brother_block);
								
								
							}
						else
							{
								//合并  默认和左兄弟合并
							
								
							combine_node(left_brother,parent,deletenode,left_brother_block,parent_block, deletenode_block,left_parent_pos);

							}
			
						}


			}


			
		
			
		}

	}


}