#include <iostream>
#include "lsb.h"

using namespace std;

int main()
{
	int id=-1;
	char bmpname[255],hidename[255];
    LSB lsb;
	cout<<"����BMPͼ�����Ϣ���ؼ���������LSB�㷨"<<endl;
	while(id)
	{
	    cout<<"+------------------------------+"<<endl;
	    cout<<"| 1.����һ���ļ�               |"<<endl;
	    cout<<"| 2.����һ��BMP�ļ�            |"<<endl;
	    cout<<"| 3.�˳�                       |"<<endl;
	    cout<<"+------------------------------+"<<endl<<endl;
		cout<<"��ѡ�������ʽ�� ";
		cin>>id;
		switch(id)
		{
		case 1:
			cout<<"������BMPͼ�������(������׺��)  ";
			cin>>bmpname;
			cout<<"�����������ļ����� ";
			cin>>hidename;
			lsb.LSBhide(bmpname,hidename);
			break;
		case 2:
			cout<<"������Ҫ���ܵ�BMPͼ�������(������׺��)  ";
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
