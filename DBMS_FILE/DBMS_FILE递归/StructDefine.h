#ifndef STRUCTDEFINE_H
#define STRUCTDEFINE_H
//  B树的阶
#define   M   2     
//记录
typedef struct _record
{
	int		phone_number;
	int		pos;	
}record;
//节点          
typedef   struct   btnode   
{   
	int	num;//节点编号  便于seek寻址
	int   count;         //   记录个数     
	record   r[2*M];         //记录         
	int   block_ptr[2*M+1];         //   指向子树的指针      
}node;
//文件头
typedef struct _configure
{
	int count;//block数
	int start_block;//root
	
	//int offset;//第一块数据的偏移
}configure; 

#endif
