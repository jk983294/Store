#ifndef CONFIGURE_H
#define CONFIGURE_H


//ʹ�õ��㷨
enum AlgorithmType{
	BrutalForce,					//������
	BranchAndBound,			//��֧���޷�
	DynamicProgramming	//��̬�滮
};
const AlgorithmType algotype = DynamicProgramming;

const bool inputHasWeight = false;				//�����ã����ϴ�Ȩֵ��txt��ʽ�� [ v1,v2 weight ]
const int POSTIVE_MAXINT = 0x7fffffff;		//�������
const int MAX_NODE_NUM = 5000;				//���ڵ���


#endif