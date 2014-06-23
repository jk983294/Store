%{
#include<iostream>
#include<string>
#include<fstream>
#include"yytab.h"
using namespace std;
%}
digit [0-9]
letter [a-zA-Z]
num  {digit}|([1-9]({digit})*)
id   {letter}({digit}|{letter})*
%%
"+="	    {return ADD_ASSIGN; }
{num}       {return NUMBER;}
break       {return BREAK; }
case        {return CASE; }
char        {return CHAR;}
const       {return CONST; }
continue    {return CONTINUE; }
default     {return DEFAULT; }
do         {return DO; }
double      {return DOUBLE; }
else        {return ELSE; }
enum        {return ENUM; }
float       {return FLOAT; }
for         {return FOR; }
goto        {return GOTO; }
if          {return IF; }
int         {return INT; }
long        {return LONG; }
return      {return RETURN; }
short       {return SHORT; }
signed      {return SIGNED; }
sizeof      {return SIZEOF; }
static      {return STATIC; }
switch      {return SWITCH; }
typedef     {return TYPEDEF; }
unsigned    {return UNSIGNED; }
void        {return VOID; }
while       {return WHILE; }
"-="	    {return SUB_ASSIGN; }
"*="	    {return MUL_ASSIGN; }
"/="	    {return DIV_ASSIGN; }
"++"	    {return INC_OP; }
"--"	    {return DEC_OP; }
"->"	    {return PTR_OP; }
"&&"	    {return AND_OP; }
"||"	    {return OR_OP; }
"<="	    {return LE_OP; }
">="	    {return GE_OP; }
"=="	    {return EQ_OP; }
"!="	    {return NE_OP; }
"="	    {return E_OP; }
"<"	    {return L_OP; }
">"	    {return G_OP; }
";"	    {return SEMICOLON; }
","	    {return COMMA; }
":"	    {return COLON; }
"."	    {return POINT; }
"&"	    {return AND; }
"!"	    {return EXCLAMATION; }
"-"	    {return DEC; }
"+"	    {return INC; }
"*"         {return SATR; }
"|"	    {return OR; }
"?"	    {return UNKNOWN; }
"{"	    {return L_BRACKETS; }
"}"         {return R_BRACKETS; }
"["         {return SL_BRACKETS; }
"]"         {return RL_BRACKETS; }
{id}        {return IDENTIFIER;}
%%
void main()
{
	string fName;       //lexÎÄ¼þÃû
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