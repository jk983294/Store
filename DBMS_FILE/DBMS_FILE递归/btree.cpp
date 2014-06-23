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

void BTree::openDB()//�������ļ�
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

void BTree::read_block(int i)//����¼��
{

	file.seekg(i*block_size);

	file.read(reinterpret_cast<char *>(&current),block_size);

}
void BTree::write_block(int i)//д��¼��
{
	file.seekp(ios::beg);// дָ��ص��ļ��ײ�	
	file.seekp(i*block_size,ios::beg);//�ƶ����ڼ�����¼

	
	file.write(reinterpret_cast<char *>(&current),block_size);
}


void BTree::initNode(){
	current.count=0;
	for (int i=0;i<2*M+1;i++)
	{
		current.block_ptr[i]=-1;
	}
}

int BTree::search(int number)                //����ָ�����,���ؽ���Ƿ����  ����-1��ʾ�����ڸýڵ���
{

    //���������ļ����Ҹ��ڵ����ڵļ�¼��
	//��ȡ��¼�鵽current����� 
	int count=conf.count; //��ȡ������Ϣ��Ŀ�����
    int start=conf.start_block;  //�������ļ��ж������ڵ����ڵĿ��
	if (count==0 && start==-1)  //һ��ʼû�и��ڵ�
	{
		return -1;       //��ʾû������
	}
	else   //����Ϊ��
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

void BTree::searchScope(int a,int b)  //������a-b֮��ĵ绰����
{
	//vector<record> find;
	set<record,setcmp> find;
	while (!path.empty())
	{
		path.pop();
	}
	int read=conf.start_block;//��ʼ�ڵ�
	int count=0;//����  �ҵ�����
	path.push(read);
	while (path.size()!=0)
	{
		read = path.top();
		path.pop();
		read_block(read);
		for (int i=0;i<current.count;i++)//ÿ����¼ֻ������ָ��   ���һ����¼������ָ��
		{
			if ((i==current.count-1) && (current.r[i].phone_number< b)
				&&(current.block_ptr[i+1] != -1))//ɨ���ý�����һ����¼  ������ָ�� 
			{
				path.push(current.block_ptr[i+1]);
			}
			if (current.r[i].phone_number>a )
			{
				if (current.r[i].phone_number<=b)
				{
					find.insert(current.r[i]);
				}				
				if (current.block_ptr[i] != -1)//������ָ��
				{
					path.push(current.block_ptr[i]);
				}
				if (current.r[i].phone_number>b)//�ҵ�һ������е�һ�����ڷ�Χ�ļ�¼  ����
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

bool BTree::get_path(int nu)     //�ڲ����ʱ��Ҫ�õ�
{
	//cout<<path.size();
	while (!path.empty())
	{
		path.pop();
	}
	
	int count=conf.count; //��ȡ������Ϣ��Ŀ�����
    int start=conf.start_block;  //�������ļ��ж������ڵ����ڵĿ��
	if (count==0 && start==-1)  //һ��ʼû�и��ڵ�
	{
		//��һ��ռ�
		initNode();
		conf.start_block=0; //���¸�ֵ
		conf.count=1;       //���¸�ֵ
		write_block(0);   //д0��
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

void BTree::insert_value(record re)     //��Ӽ�¼
{
	//��Ӽ�¼  ���Ȳ����Ƿ���ڸü�¼  ����·����¼��pathջ��
	bool isExist=get_path(re.phone_number);
	if (isExist)
	{
		cout<<"�ýڵ��Ѵ���"<<endl;
		return; 
	}
	else{    //�ýڵ㲻���������ڵ���·����¼��path��
        int number=path.top();//��ȡ���һ��������ڵĿ��
		path.pop();   
		read_block(number);  //��ȡ��Ϣ
		if (current.count==2*M)  //���������
		{
			//���ѽ��
			split_node(re,number,-1,-1);  
		   
			
			
		}
		else
		{
			//ͨ���Ƚϲ���
			if (current.count==0)
			{
				//ֱ�Ӳ���
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
				
            } //�ƶ�λ��
			current.r[pos]=re;
			current.count++;
			write_block(number);  //д�ؿ����Ϣ
		}
		
	}
   

}


void BTree::split_node(record re,int block,int left,int right)     //���ѽ��
{
	int root;
	//cout<<path.size()<<endl;

    read_block(block);  //��ȡ����Ϣ
	if (current.count==2*M)  //���������
	{
		//���ѽ��
		//ȷ��Ҫ�����λ��
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
		
		if (pos==M)   //�������λ�����м��򽫲���Ľ���ֵ��Ϊ���׽��
		{
			//�����������  �޸�block�������  ����µĽ��
			//node *newnode=new node;
			newnode=current;
			//newnode=&current;  //
			current.count=M;
			current.block_ptr[M]=left;
			write_block(block);  //Э�����
            conf.count++;   //������1
			initNode(); //��������һ�����
            current.count=M;
			//��ʼ��ֵ
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
				root =path.top();  //�õ���һ�������
		        path.pop();
                split_node(re,root,block,conf.count-1);    
			}
		}
		else if (pos<M)   //��m-1λ���ϵļ�¼��Ϊ���׽��
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
            //����ָ��
            current.block_ptr[pos]=left;
			current.block_ptr[pos+1]=right;

			write_block(block);  //Э�����
            conf.count++;
			initNode(); //��������һ�����
            current.count=M;
			//��ʼ��ֵ
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
				root =path.top();  //�õ���һ�������
				path.pop();
                split_node(*parent,root,block,conf.count-1);       
			}
			
			
			

		}
		else if (pos>M)   //��Mλ���ϵļ�¼��Ϊ���׽��
		{
			record *parent=new record;
			*parent=current.r[M];
			//node *newnode=new node;
			newnode=current;  //
			current.count=M;
			//
			write_block(block);  //Э�����
            conf.count++;
			initNode(); //��������һ�����
            current.count=M;
			//��ʼ��ֵ
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
				root =path.top();  //�õ���һ�������
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
				
            } //�ƶ�λ��

			current.count++;
			
			current.r[pos]=re;
			current.block_ptr[pos]=left;
			current.block_ptr[pos+1]=right;
			
			write_block(block);  //д�ؿ����Ϣ

		}
}
void BTree::insertRange(int range){
	Sleep(1000*60*1);
}
int BTree::get_path1(int number)   //���ظ��������ǿ���  ����·����¼����
{
	while (!path.empty())
	{
		path.pop();
	}
	
	int count=conf.count; //��ȡ������Ϣ��Ŀ�����
    int start=conf.start_block;  //�������ļ��ж������ڵ����ڵĿ��
	if (count==0 && start==-1)  //һ��ʼû�и��ڵ�
	{
		return -1;   //������
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
void BTree::del_value(int nu)        //ɾ����¼
{
	int root,pos;
	int i,j;
	int parent_block,left_brother_block,right_brother_block,deletenode_block;
	int left_parent_pos,right_parent_pos;

	//ɾ����¼  ���Ȳ����Ƿ���ڸü�¼  ����·����¼��pathջ��
	int isExist=get_path1(nu);
	if (isExist==-1)
	{
		cout<<"�ýڵ㲻����"<<endl;
		return; 
	}
	else{    //�ýڵ����
		//�жϸý���Ƿ���Ҷ�ڵ㣻
        bool leaf=isLeaf(isExist);
        if (leaf)   //��Ҷ�ӽ��
        {
            int number=path.top();//Ҫɾ���Ŀ��
			path.pop();   
		    read_block(number);  //��ȡ��Ϣ
			deletenode=current;
			deletenode_block=number;
			if (conf.start_block==number)   //ֻ�и��ڵ�
			{
				//ֱ��ɾ��
				//ֱ��ɾ��
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
					
				} //�ƶ�λ��
				current.count--;
				write_block(number);  //д�ؿ����Ϣ

			}
			else        //�Ǹ����
			{
				if (current.count>M)
				{
					//ֱ��ɾ��
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
						
					} //�ƶ�λ��
					current.count--;
					write_block(number);  //д�ؿ����Ϣ
					
				}
				else 
				{
					//��ֱ��ɾ��  ���ֵܽ��
					//�Ҹ��׽��
					root=path.top();
					path.pop();
					read_block(root);
					parent=current;  //���׽��
					parent_block=root;
					//���ֵܽ��  ���ж��Լ����Ǹ�λ��
					for (i=0;i<=current.count;i++)
					{
						if (current.block_ptr[i]==number)
						{
							pos=i;
							break;
						}
                    
					}
                    //pos��ָ���λ��
					if (pos==0)    //�Լ��ǵ�һ������  û�����ֵ�
					{
						right_parent_pos=pos;
						read_block(parent.block_ptr[pos+1]);   //�������ֵ�
						right_brother=current;
						right_brother_block=parent.block_ptr[pos+1];
						if (current.count>M)
						{
							//����һ������  
							//���׽������Լ� 
							//�Ƚ��Ǹ�����ɾ��  
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
								
							} //�ƶ�λ��
							//���븸�׽��
							deletenode.r[deletenode.count-1]=parent.r[right_parent_pos];

							parent.r[right_parent_pos]=right_brother.r[0];

							//���ֵ���ɾ�����0
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
						{    //û�ж���ĺ���  ֻ�ܺϲ�  
							//�Ƚ��Ǹ���¼ɾ��
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
								
							} //�ƶ�λ��
							deletenode.count--;

							combine_node(deletenode,parent,right_brother,deletenode_block,parent_block,right_brother_block,right_parent_pos);
							




						}

					}
					else if (pos==parent.count)
					{
						//���һ������
						left_parent_pos=pos-1;
						read_block(current.block_ptr[pos-1]);   //�������ֵ�
						left_brother=current;
						left_brother_block=parent.block_ptr[pos-1];
						if (current.count>M)
						{
							//����һ������  
							//���׽������Լ� 
							//�Ƚ��Ǹ�����ɾ��  
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
								
							} //�ƶ�λ��
							//���븸�׽��
							for (i=deletenode.count;i>=1;i--)
							{
								deletenode.r[i]=deletenode.r[i-1];
							}
							deletenode.r[0]=parent.r[left_parent_pos];
							
							parent.r[left_parent_pos]=left_brother.r[left_brother.count-1];
							
							//���ֵ���ɾ�����0
							left_brother.count--;
							
							
							current=parent;
							write_block(parent_block);
							
							current=deletenode;
							write_block(deletenode_block);
							
							current=left_brother;
							write_block(left_brother_block);
							
							
						}
						else
						{    //û�ж���ĺ���  ֻ�ܺϲ�
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
								
							} //�ƶ�λ��
							deletenode.count--;
							
							combine_node(left_brother,parent,deletenode,left_brother_block,parent_block, deletenode_block,left_parent_pos);
							
							
						}


					}
					else          //�������� �����Һ���
					{
						//�ȶ�������
						left_parent_pos=pos-1;
						read_block(current.block_ptr[pos-1]);   //�������ֵ�
						left_brother=current;
						left_brother_block=parent.block_ptr[pos-1];
						if (current.count>M)
						{
							//����һ������  
							//���׽������Լ� 
							//�Ƚ��Ǹ�����ɾ��  
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
								
							} //�ƶ�λ��
							//���븸�׽��
							for (i=deletenode.count;i>=1;i--)
							{
								deletenode.r[i]=deletenode.r[i-1];
							}
							deletenode.r[0]=parent.r[left_parent_pos];
							
							parent.r[left_parent_pos]=left_brother.r[left_brother.count-1];
							
							//���ֵ���ɾ�����0
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
							//û���㹻�ָ��ֵ�
							right_parent_pos=pos;
							read_block(parent.block_ptr[pos+1]);   //�������ֵ�
							right_brother=current;
							right_brother_block=parent.block_ptr[pos+1];
							if (current.count>M)
							{
								//����һ������  
								//���׽������Լ� 
								//�Ƚ��Ǹ�����ɾ��  
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
									
								} //�ƶ�λ��
								//���븸�׽��
								deletenode.r[deletenode.count-1]=parent.r[right_parent_pos];
								
								parent.r[right_parent_pos]=right_brother.r[0];
								
								//���ֵ���ɾ�����0
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
								//�ϲ�  Ĭ�Ϻ����ֵܺϲ�
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
									
								} //�ƶ�λ��
								deletenode.count--;
								
							combine_node(left_brother,parent,deletenode,left_brother_block,parent_block, deletenode_block,left_parent_pos);

							}
						}

					}
                    
					
					
					
				}

			}
		
        }
		else        //��Ҷ�ӽ��
		{
			//���������ĵ���СԪ�ش��汻ɾ����Ԫ��
			//�������������СԪ��
			int number =path.top();    //����м���ı��
			path.pop();
			//��λ�Լ�
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
			//��ü�¼
			record *re=new record;
			*re=current.r[0];
			int phonenumber=current.r[0].phone_number;

			

			del_value(phonenumber);

			//����Ҫɾ���ļ�¼
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

void BTree::combine_node(node left,node parent,node right,int left_block,int parent_block,int right_block,int parent_pos)   //�ϲ����
{
	int root,pos;
	int i,j;
	int parent_block1,left_brother_block,right_brother_block,deletenode_block;
	int left_parent_pos,right_parent_pos;

    if (parent_block==conf.start_block)   //�����Ǹ��ڵ�
    {
		if (parent.count==1)
		{
			//ֱ�Ӻϲ�
			left.count++;
	        left.r[left.count-1]=parent.r[parent_pos];//�����׽��ϲ�

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



			//���ֵ�
			
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
			left.r[left.count-1]=parent.r[parent_pos];//�����׽��ϲ�
			
			//���׽�㱻�ı�
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
			
			//���ֵ�
			
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
	else    //���Ǹ����
	{
		//�����Ǹ����Ļ�  ������M�����
		//�Ⱥϲ�
		
		
		left.count++;
		left.r[left.count-1]=parent.r[parent_pos];//�����׽��ϲ�
		
		//���׽�㱻�ı�
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
		
		//���ֵ�
		
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
			//����
			return;
		}
		else
		{
			//�����ٺϲ�
			//���Ǹ����  ���
			deletenode=parent;
			deletenode_block=parent_block;

			root=path.top();
			path.pop();
			read_block(root);  
			parent=current;

			//��λ
			parent_block=root;  //
			//���ֵܽ��  ���ж��Լ����Ǹ�λ��
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
				read_block(parent.block_ptr[pos+1]);   //�������ֵ�
				right_brother=current;
				right_brother_block=parent.block_ptr[pos+1];
				if (current.count>M)
				{
					//����һ������  
					//���׽������Լ� 
					//���븸�׽��
					deletenode.count++;
					deletenode.r[deletenode.count-1]=parent.r[right_parent_pos];
					
					parent.r[right_parent_pos]=right_brother.r[0];

					deletenode.block_ptr[deletenode.count]=right_brother.block_ptr[0];
					
					//���ֵ���ɾ�����0
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
				{    //û�ж���ĺ���  ֻ�ܺϲ�  
					//�Ƚ��Ǹ���¼ɾ��
					
					combine_node(deletenode,parent,right_brother,deletenode_block,parent_block,right_brother_block,right_parent_pos);
					
					
					
					
					
				}

			}
			else if (pos==parent.count)
			{
				//���һ������
				left_parent_pos=pos-1;
				read_block(current.block_ptr[pos-1]);   //�������ֵ�
				left_brother=current;
				left_brother_block=parent.block_ptr[pos-1];
				if (current.count>M)
				{
					//����һ������  
					//���׽������Լ� 
					//���븸�׽��
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
					
					//���ֵ���ɾ�����0
					left_brother.count--;
					
					
					current=parent;
					write_block(parent_block);
					
					current=deletenode;
					write_block(deletenode_block);
					
					current=left_brother;
					write_block(left_brother_block);
					
					
				}
				else
				{    //û�ж���ĺ���  ֻ�ܺϲ�
			
					
					combine_node(left_brother,parent,deletenode,left_brother_block,parent_block, deletenode_block,left_parent_pos);
					
					
				}

			}
			else
			{
				//�ȶ�������
				left_parent_pos=pos-1;
				read_block(current.block_ptr[pos-1]);   //�������ֵ�
				left_brother=current;
				left_brother_block=parent.block_ptr[pos-1];
				if (current.count>M)
				{
					//����һ������  
					//���׽������Լ� 
					//���븸�׽��
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
					
					//���ֵ���ɾ�����0
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
							//û���㹻�ָ��ֵ�
							//���븸�׽��
							right_parent_pos=pos;
							read_block(parent.block_ptr[pos+1]);   //�������ֵ�
							right_brother=current;
							right_brother_block=parent.block_ptr[pos+1];
							if (current.count>M)
							{
								//����һ������  
								//���׽������Լ� 
								//���븸�׽��
								deletenode.count++;
								deletenode.r[deletenode.count-1]=parent.r[right_parent_pos];
								
								parent.r[right_parent_pos]=right_brother.r[0];
								
								deletenode.block_ptr[deletenode.count]=right_brother.block_ptr[0];
								
								//���ֵ���ɾ�����0
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
								//�ϲ�  Ĭ�Ϻ����ֵܺϲ�
							
								
							combine_node(left_brother,parent,deletenode,left_brother_block,parent_block, deletenode_block,left_parent_pos);

							}
			
						}


			}


			
		
			
		}

	}


}