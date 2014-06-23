%{
#include<iostream>
#include<string>
#include<map>
#include"yytab.h"
using namespace std;
%}
%%
"*"        {return STAR;}
"="        {return EQ;}
"i"          {return I; }
%%
