%{
#include<iostream>
#include<string>
#include<fstream>
#include"yytab.h"
using namespace std;
%}
%%
break      {return BREAK; }
char        {return CHAR;}
const       {return CONST; }
continue    {return CONTINUE; }
default     {return DEFAULT; }
%%
void main()
{
	string fName;       //lexÎÄ¼şÃû
	cout<<"Please enter the name of application_file:";
	cin>>fName;
	ifstream ifile;
	ifile.open(fName.c_str());
	if(!ifile.good())
	{
		cerr<<"Open the file error"<<endl;	
		exit(1);
	}
	cout<<"Open the file "<<fName<<" succeed"<<endl;
	while(!ifile.eof()){
		string str;
		ifile>>str;
		int index=analysis(str.c_str(),strlen(str.c_str()));
        cout<<index<<endl;
	}
}