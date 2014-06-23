%{
#include<iostream>
#include<string>
#include<map>
#include"yytab.h"
using namespace std;
%}
digit [0-9]
letter [a-zA-Z]
num  [1-9]({digit})*
id   {letter}({digit}|{letter})*
%%
"-"	    {return SUB; }
"+"	    {return ADD; }
"*"         {return MUL; }
"/"         {return DIV;}
"0"         {return NUM;}
"("         {return SL_BRACKETS;}
")"         {return SR_BRACKETS;}
"="         {return EQ;}
{num}       {return NUM;}
{id}        {return IDENTIFIER;}
%%
