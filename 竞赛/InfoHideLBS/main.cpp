#include <iostream>
#include "lsb.h"

using namespace std;

int main()
{
	int id=-1;
	char bmpname[255],hidename[255];
    LSB lsb;
	cout<<"单张BMP图像的信息隐藏技术，采用LSB算法"<<endl;
	while(id)
	{
	    cout<<"+------------------------------+"<<endl;
	    cout<<"| 1.加密一个文件               |"<<endl;
	    cout<<"| 2.解密一个BMP文件            |"<<endl;
	    cout<<"| 3.退出                       |"<<endl;
	    cout<<"+------------------------------+"<<endl<<endl;
		cout<<"请选择操作方式： ";
		cin>>id;
		switch(id)
		{
		case 1:
			cout<<"请输入BMP图像的名字(包含后缀名)  ";
			cin>>bmpname;
			cout<<"请输入隐藏文件名： ";
			cin>>hidename;
			lsb.LSBhide(bmpname,hidename);
			break;
		case 2:
			cout<<"请输入要解密的BMP图像的名字(包含后缀名)  ";
			cin>>bmpname;
			lsb.DeLSBhide(bmpname);
			break;
		default:
			exit(1);
		}
	}
	system("pause");
	return 0;
}
