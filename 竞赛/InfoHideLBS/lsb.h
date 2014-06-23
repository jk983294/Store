#ifndef _LSB_H
#define _LSB_H

#include "windows.h"
#include <fstream>
#include <cmath>

using namespace std;

class LSB
{
private:
	BITMAPFILEHEADER *m_bitmapfileheader;    //ָ��BMPͼ���ļ�ͷ��ָ��
	BITMAPINFOHEADER *m_bitmapinfoheader;    //ָ��BMPͼ����Ϣͷ��ָ��
	RGBQUAD *m_colortable;                  //ָ��BMPͼ���ɫ���ָ��
	char *m_data;							//ָ��BMPͼ����������ָ��
	int max_bitmaplength;					//BMPͼ�����������Ϣ��

	char *strByteBuffer;					//�����ļ��Ļ�����
	int hidelength;							//�����ļ��ĳ���
public:
	LSB();
	~LSB();
	void readBMP(char * bmpname);						//��ȡBMPͼ���ڴ�
	void readHide(char *hidename);						//��ȡ�����ļ���������
	void LSBhide(char * bmpname,char * hidename);		//��LSB�㷨�����ļ�
	void DeLSBhide(char * bmpname_hide);			   //��ȡ�����ļ���Ϣ
	void createBMP(char *createbmp);					//�����Ѿ���������Ϣ��BMPͼ��
	void createFile(char *getfile,char *strByteBuffer,int HideLength);//������ȡ��Ϣ���ļ�
};

#endif