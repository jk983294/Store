#ifndef STRUCTDEFINE_H
#define STRUCTDEFINE_H
//  B���Ľ�
#define   M   2     
//��¼
typedef struct _record
{
	int		phone_number;
	int		pos;	
}record;
//�ڵ�          
typedef   struct   btnode   
{   
	int	num;//�ڵ���  ����seekѰַ
	int   count;         //   ��¼����     
	record   r[2*M];         //��¼         
	int   block_ptr[2*M+1];         //   ָ��������ָ��      
}node;
//�ļ�ͷ
typedef struct _configure
{
	int count;//block��
	int start_block;//root
	
	//int offset;//��һ�����ݵ�ƫ��
}configure; 

#endif
