#include<iostream>
#include<fstream>
#include"math.h"
#include"lsb.h"
#include"windows.h"

using namespace std;

//���幹�캯��
LSB::LSB()
{
	m_bitmapfileheader=new BITMAPFILEHEADER;
	m_bitmapinfoheader=new BITMAPINFOHEADER;
	m_colortable=NULL;
	m_data=NULL;
	max_bitmaplength=0;

	char *strByteBuffer=NULL;
	int hidelength=0;
}
//������������
LSB::~LSB()
{
	delete m_bitmapfileheader;
	delete m_bitmapinfoheader;
	delete m_colortable;
	delete m_data;
	delete strByteBuffer;
}

//��ȡBMPͼ���ڴ�
void LSB::readBMP(char *bmpname)
{
	ifstream file;
	char m_bmpname[255];
	strcpy(m_bmpname,bmpname);
	int biBitCount; //BMPͼ������

	file.open(m_bmpname, ios::out | ios::in | ios::binary);
	file.read((char *)m_bitmapfileheader,sizeof(BITMAPFILEHEADER));
	file.read((char *)m_bitmapinfoheader,sizeof(BITMAPINFOHEADER));
    biBitCount=m_bitmapinfoheader->biBitCount;
	if(biBitCount==2||biBitCount==4||biBitCount==8)
	{
		m_colortable=new RGBQUAD[pow(2,biBitCount)];
		file.read((char *)m_colortable,pow(2,biBitCount)*sizeof(RGBQUAD));
	}
	m_data=new char[(m_bitmapinfoheader->biSizeImage+1)];
	file.read((char *)m_data,m_bitmapinfoheader->biSizeImage);
	max_bitmaplength=(m_bitmapinfoheader->biSizeImage)/8;
	cout<<"��BMPͼ����������ص���Ϣ��Ϊ�� "<<max_bitmaplength-32<<endl;
}

//��ȡ�����ļ���������
void LSB::readHide(char *hidename)
{
	ifstream hidefile;
	hidefile.open(hidename, ios::binary | ios::in);
	hidefile.seekg(0,hidefile.end);
	hidelength=hidefile.tellg();
	cout<<"�������ļ���Ϣ��Ϊ�� "<<hidelength<<endl;

	//�ж��Ƿ��������Ϣ���أ��������˳�����
	if(max_bitmaplength<hidelength)
	{
		cout<<"���ص��ļ���Ϣ������"<<endl;
		return;
	}

	//�������ļ���Ϣ���뻺����
	strByteBuffer=new char[hidelength];
	hidefile.seekg(0,hidefile.beg);
	hidefile.read((char *)strByteBuffer,hidelength);
	hidefile.close();
}
//�����Ѿ���������Ϣ��BMPͼ��
void LSB::createBMP(char *createbmp)
{
	ofstream dibFile;
	int biBitCount;
	dibFile.open(createbmp, ios::out | ios::binary);

	dibFile.write((char*)m_bitmapfileheader,sizeof(BITMAPFILEHEADER));
	dibFile.write((char*)m_bitmapinfoheader,sizeof(BITMAPINFOHEADER));
    biBitCount=m_bitmapinfoheader->biBitCount;
	if(biBitCount==2||biBitCount==4||biBitCount==8)
	{
		dibFile.write((char *)m_colortable,pow(2,biBitCount)*sizeof(RGBQUAD));
	}
	dibFile.write((char *)m_data,m_bitmapinfoheader->biSizeImage);
	dibFile.close();
	cout<<"������Ϣ���"<<endl<<endl<<endl;
}

//������ȡ��Ϣ���ļ�
void LSB::createFile(char *getfile,char *strByteBuffer,int HideLength)
{
	ofstream outfile;
	outfile.open(getfile, ios::out | ios::binary);
	outfile.write((char *)strByteBuffer,HideLength);
	outfile.close();
	delete strByteBuffer;
	cout<<"��Ϣ����ȡ"<<endl;
}

//��LSB�㷨�����ļ�
void LSB::LSBhide(char *bmpname,char *hidename)
{
    int i,j;
	char createbmp[255];

	readBMP(bmpname);
	readHide(hidename);

	//BMPͼ����������ǰ32���ֽ�������¼�����ļ����ݳ���
	for(i=0;i<32;i++)
	{
		m_data[i]= ((hidelength>>(31-i)) & 0x00000001)+(m_data[i]&0xfe);
	}

	//����LSB�㷨�����ļ���Ϣ
	for(i=0;i<hidelength;i++)
	{
		for(j=0;j<8;j++)
		{
			m_data[i*8+j+32]=((strByteBuffer[i]>>(7-j))& 0x01)+(m_data[i*8+j+32]&0xfe);
		}
	}
	cout<<"����������Ϣ��BMPͼ������ ";
	cin>>createbmp;
	createBMP(createbmp);

}

//��ȡ�����ļ���Ϣ
void LSB::DeLSBhide(char *bmpname_hide)
{
	int HideLength=0;//���������Ҫ
	int i,j;
	char getfile[255];


	readBMP(bmpname_hide);

	//��BMPͼ����������ǰ32���ֽڶ�ȡ�ļ��ĳ���
	for(i=0;i<32;i++)
	{
		HideLength+=(m_data[i]&0x01)<<(31-i);
	}

	char *DestrByteBuffer=NULL;
	DestrByteBuffer=new char[HideLength];
	for(i=0;i<HideLength;i++)
	{
		DestrByteBuffer[i]=0;
		for(j=0;j<8;j++)
		{
			DestrByteBuffer[i]+=((m_data[i*8+j+32]&0x01)<<(7-j));
		}
	}

	cout<<"������ȡ��Ϣ���ļ�����  ";
	cin>>getfile;
	createFile(getfile,DestrByteBuffer,HideLength);
}