#include<iostream>
#include<fstream>
#include"math.h"
#include"lsb.h"
#include"windows.h"

using namespace std;

//定义构造函数
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
//定义析构函数
LSB::~LSB()
{
	delete m_bitmapfileheader;
	delete m_bitmapinfoheader;
	delete m_colortable;
	delete m_data;
	delete strByteBuffer;
}

//读取BMP图像到内存
void LSB::readBMP(char *bmpname)
{
	ifstream file;
	char m_bmpname[255];
	strcpy(m_bmpname,bmpname);
	int biBitCount; //BMP图像的深度

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
	cout<<"该BMP图像最大能隐藏的信息量为： "<<max_bitmaplength-32<<endl;
}

//读取隐藏文件到缓冲区
void LSB::readHide(char *hidename)
{
	ifstream hidefile;
	hidefile.open(hidename, ios::binary | ios::in);
	hidefile.seekg(0,hidefile.end);
	hidelength=hidefile.tellg();
	cout<<"该隐藏文件信息量为： "<<hidelength<<endl;

	//判断是否能完成信息隐藏，若不能退出程序
	if(max_bitmaplength<hidelength)
	{
		cout<<"隐藏的文件信息量过大"<<endl;
		return;
	}

	//将隐藏文件信息读入缓冲区
	strByteBuffer=new char[hidelength];
	hidefile.seekg(0,hidefile.beg);
	hidefile.read((char *)strByteBuffer,hidelength);
	hidefile.close();
}
//创建已经隐藏了信息的BMP图像
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
	cout<<"隐藏信息完毕"<<endl<<endl<<endl;
}

//创建提取信息的文件
void LSB::createFile(char *getfile,char *strByteBuffer,int HideLength)
{
	ofstream outfile;
	outfile.open(getfile, ios::out | ios::binary);
	outfile.write((char *)strByteBuffer,HideLength);
	outfile.close();
	delete strByteBuffer;
	cout<<"信息已提取"<<endl;
}

//用LSB算法隐藏文件
void LSB::LSBhide(char *bmpname,char *hidename)
{
    int i,j;
	char createbmp[255];

	readBMP(bmpname);
	readHide(hidename);

	//BMP图像数据区的前32个字节用来记录隐藏文件数据长度
	for(i=0;i<32;i++)
	{
		m_data[i]= ((hidelength>>(31-i)) & 0x00000001)+(m_data[i]&0xfe);
	}

	//基于LSB算法隐藏文件信息
	for(i=0;i<hidelength;i++)
	{
		for(j=0;j<8;j++)
		{
			m_data[i*8+j+32]=((strByteBuffer[i]>>(7-j))& 0x01)+(m_data[i*8+j+32]&0xfe);
		}
	}
	cout<<"输入隐藏信息的BMP图像名： ";
	cin>>createbmp;
	createBMP(createbmp);

}

//提取隐藏文件信息
void LSB::DeLSBhide(char *bmpname_hide)
{
	int HideLength=0;//等于零很重要
	int i,j;
	char getfile[255];


	readBMP(bmpname_hide);

	//从BMP图像数据区的前32个字节读取文件的长度
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

	cout<<"输入提取信息的文件名：  ";
	cin>>getfile;
	createFile(getfile,DestrByteBuffer,HideLength);
}