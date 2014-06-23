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
char        {return CHAR;}
do          {return DO;}
double      {return DOUBLE; }
else        {return ELSE; }
float       {return FLOAT; }
for         {return FOR; }
if          {return IF; }
int         {return INT; }
long        {return LONG; }
short       {return SHORT; }
signed      {return SIGNED; }
unsigned    {return UNSIGNED;}
while       {return WHILE; }
"=="	    {return EQ_OP; }
"!="	    {return NE_OP; }
"="	    {return E_OP; }
"<"	    {return L_OP; }
">"	    {return G_OP; }
">="        {return GE_OP;}
"<="        {return LE_OP;}
";"	    {return SEMICOLON; }
","	    {return COMMA; }
"-"	    {return SUB; }
"+"	    {return ADD; }
"*"         {return MUL; }
"/"         {return DIV;}
"{"	    {return L_BRACKETS; }
"}"         {return R_BRACKETS; }
"("         {return SL_BRACKETS;}
")"         {return SR_BRACKETS;}
"#"         {return TERMINAL;}
"0"         {return NUM;}
{num}       {return NUM;}
{id}        {return IDENTIFIER;}
%%
