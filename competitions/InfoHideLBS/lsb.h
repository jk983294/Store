#ifndef _LSB_H
#define _LSB_H

#include "windows.h"
#include <fstream>
#include <cmath>

using namespace std;

class LSB
{
private:
	BITMAPFILEHEADER *m_bitmapfileheader;    //指向BMP图像文件头的指针
	BITMAPINFOHEADER *m_bitmapinfoheader;    //指向BMP图像信息头的指针
	RGBQUAD *m_colortable;                  //指向BMP图像调色板的指针
	char *m_data;							//指向BMP图像数据区的指针
	int max_bitmaplength;					//BMP图像最大隐藏信息量

	char *strByteBuffer;					//隐藏文件的缓冲区
	int hidelength;							//隐藏文件的长度
public:
	LSB();
	~LSB();
	void readBMP(char * bmpname);						//读取BMP图像到内存
	void readHide(char *hidename);						//读取隐藏文件到缓冲区
	void LSBhide(char * bmpname,char * hidename);		//用LSB算法隐藏文件
	void DeLSBhide(char * bmpname_hide);			   //提取隐藏文件信息
	void createBMP(char *createbmp);					//创建已经隐藏了信息的BMP图像
	void createFile(char *getfile,char *strByteBuffer,int HideLength);//创建提取信息的文件
};

#endif