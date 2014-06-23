#pragma warning(disable:4786)
#include<iostream>
#include<fstream>
#include<string>
#include<map>
#include <set>
#include<vector>
#include<stack>
#include<list>
#include<queue>
#include<time.h>
#include "StructDefine.h"
using namespace std;
//using   namespace   stdext;
//全局变量
ofstream ofile;      
ifstream ifile;
int linenum=0;                  //记录行号
int stateID=0;                  //状态计数
int stateMiniDFA=0;             //最小化dfa状态计数
set<int> symbolTable;           //输入的符号表,用于存放触边的各种可能
map<string,string> id2reTable;  //自定义标识符名到正规表达式的映射
map<int,int> nfaTeminal;        //NFA终态对应的动作表
map<int,int> dfaTeminal;        //DFA终态对应的动作表
map<int,int> mdfaTeminal;       //最小化DFA终态对应的动作表
//map<int> minidfaTeminal;        //记录最小化的终结太
vector<string> actionTable;     //动作表
vector<string> reTable;         //正规表达式表
stack<NFA> nfaStack;            //NFA栈，由正规表达式构造NFA时要用到
map<set<int>,int> dStates;      //DFA节点的状态标记
NFA nfa;                        //最终的NFA
DFA dfa;                        //最终的DFA
MDFA mdfa;                      //最小化DFA
DFA mdfa1;                      //最小化标准的DFA
time_t begin,end;               //测试时间
void Init();                     //初始化,打开输入输出文件
void AnalysisDefineSegment();   //扫描定义段
void AnalysisRuleSegment();     //扫描规则段
void AnalysisProgramSegment();  //输出例程序
void ConstructNFA();             //构造NFA

int  CheckSpecialmark(char c);     //特殊符号检测

void  replaceRe(string &re,map<string,string> &id2reTable);//替换子表达式
void  completRe(string &re);//完整表达式 加连接符@
void  postFix(string &re);//中缀变后缀
int isp(char x);//栈内优先级
int icp(char x);//入栈优先级
bool isAlphanum(char x);//判断是否为数字或字母

void Thompson(string &re,NFA &tnfa); //Thompson构造法
void Join(NFA &tnfa);     //合并两个NFA
bool Pop(NFA &tnfa);      //NFA栈的出栈操作
void Push(char chInput);  //NFA栈的入栈操作

void NFAtoDFA();          //将NFA转化为相应的DFA
set<int> EpsilonClosure(set<int> &T); //求EpsilonClosure闭包
set<int> MoveTo(set<int> &T,int a);  //求MoveTo(T,a)
bool GetUnmarked(map<set<int>,int>::iterator &m_iter); //获得未被标记DFA的状态
multimap<int,int> GetTable(int state);   //从nfa中获得状态的映射表
multimap<int,int> GetTable1(int state);   //从dfa中获得状态的映射表
int dfaIsTerminated(set<int> &T);   //判断dfa的一个状态集是不是终态状态集
int mdfaIsTerminated(set<int> &T);  //判断最小化dfa一个状态集是不是终态状态集

void miniDFA();  //最小化DFA
int SearchCollectNumber(int number);  //判断该状态在哪个集合中，返回集合号
int getMiniStartState();  //用于确定最小化dfa的起始结点


void GenerateCode(DFA &dfa);//根据DFA生成代码
bool FindSameState(multimap<int,int> &table,int &s);//生成代码时简化代码
int getMiniStartState();  //用于确定最小化dfa的起始结点

int main()
{
	Init();                           
	AnalysisDefineSegment();                 
	AnalysisRuleSegment();
	begin=clock();
	ConstructNFA();
	end=clock();
	cout<<"runtime: "<<double(end-begin)/CLOCKS_PER_SEC<<endl;
	begin=clock();
	NFAtoDFA();          //NFA转换为DFA
	end=clock();
	cout<<"runtime: "<<double(end-begin)/CLOCKS_PER_SEC<<endl;
	begin=clock();
	miniDFA();
	end=clock();
	cout<<"runtime: "<<double(end-begin)/CLOCKS_PER_SEC<<endl;
    GenerateCode(mdfa1);   //产生yylex.cpp
    AnalysisProgramSegment();
	ifile.close();
	ofile.close();
	return 0;
}

void Init()     //初始化函数
{
	string fName;       //lex文件名
	cout<<"Please enter the name of l file:";
	cin>>fName;
	ifile.open(fName.c_str());  
	ofile.open("yylex.cpp",ios::out);
	if(!ofile.good()||!ifile.good())
	{
		cerr<<"Open the file error"<<endl;	
		exit(1);
	}
	cout<<"Open the file "<<fName<<" succeed"<<endl;	
}
int CheckSpecialmark(char c)
{
	if(c=='%')
	{
		char cc=ifile.get();
		switch(cc)
		{
		case '%':
			return LAYER_ID;
		case '{':
			return HEADER_BEGIN;
		case '}':
			return HEADER_END;
		default:
			ifile.putback(cc);
			break;
		}
	}
	return ERROR;
}
void AnalysisDefineSegment()  //扫描定义段
{
	cout<<"Analysis defintion segment."<<endl;
	char c=ifile.get();
	int state=CheckSpecialmark(c);
	if(state!=HEADER_BEGIN)//判断开头是不是%{
	{
		cerr<<"line "<<linenum<<":Not begin with %{"<<endl;
		exit(1);
	}
    c=ifile.get(); 
	if(c!='\n') 
		ifile.putback(c);  
	else 
		linenum++;
	while(!ifile.eof())	//判断到%}或到文件尾为止进行扫描
	{
		c=ifile.get();
		if(c=='\t') 
			continue;       //跳过\t字符不输出。
		else if(c=='\n') 
			linenum++; //让行号自增，用以判断错误行号。
		else 
			if(c=='%')            //当接受到%时，注意判断是不是特殊符号
		{
			state=CheckSpecialmark(c);
			if(state!=HEADER_END)  //判断结束是不是%}
			{
				cerr<<"line "<<linenum<<":Not end with %}"<<endl;
				exit(1);
			}
			break;
		}
		ofile.put(c);
	}//end while
	

    c=ifile.get(); 
	if(c!='\n') 
		ifile.putback(c);
	else 
		linenum++;
	//开始读入自定义的内容
	pair<string,string> pi;	
	while(!ifile.eof())
	{
		c=ifile.get();
		if(c!='%')
		{
			ifile.putback(c);//如果不是特殊字符%，把当前字符放回流中。
		}
		else
		{
			state=CheckSpecialmark(c);
			if(state!=LAYER_ID)   //用户自定义的标识符不可以含有此特殊字符%。
			{
				cerr<<"line "<<linenum<<":Defintion error"<<endl;
				exit(1);
			}
			break;//跳过下面的正常串的处理，因为已经到终结。
		}
		string id,re;
		ifile>>id>>re;
		pi.first=id;
		pi.second=re;
		id2reTable.insert(pi);	//正规表达式	
		ifile.get();
		linenum++;
	}//end while
	cout<<"Analysis defintion segment finish."<<endl;
}

void AnalysisRuleSegment() //扫描规则段
{
	cout<<"Analysis rule segment..."<<endl;
    char c=ifile.get();   
	if(c!='\n')
		ifile.putback(c);  
	else 
		linenum++;
	while(!ifile.eof())
	{
		c=ifile.get();
		if(c!='%')
		{
			ifile.putback(c);//如果不是特殊字符%，把当前字符放回流中
		}
		else
		{
			int state=CheckSpecialmark(c);
			if(state!=LAYER_ID)   
			{
				cerr<<"line "<<linenum<<":Defintion error"<<endl;
				exit(1);
			}
			break;//跳过下面的正常串的处理，因为已经到终结。
		}

		string re,action;		
		
		ifile>>re>>action;
		c=ifile.get();
		while (c!='}')
		{
			action = action+c;
			c=ifile.get();
		}
		action = action +c;
		actionTable.push_back(action);//将动作加入到表中
		reTable.push_back(re);
		c=ifile.get();
		linenum++;
	}
	cout<<"Analysis rule segment finish"<<endl;
}
void  replaceRe(string &re,map<string,string> &id2reTable)
{
	stack<int> status;//状态栈
	status.push(-1);
	unsigned int i=0,j=0;
	while(i<re.size())
	{
		if(re[i]=='{')
			status.push(i);//记录下此时的位置
		if(re[i]=='}')
		{
			int prei=status.top();
			if(prei<0) 
			{
				i++;
				continue;//表示此时的}并无配对，因此直接输出
			}
			int length=i-prei-1;
			string id=re.substr(prei+1,length);
			string replacestr=id2reTable[id];
			if(replacestr.empty()) 
			{
				cout<<"replace error"<<endl;
				exit(0);
			}//表示其中的内容不是标识符，忽略不处理
			re.replace(prei,length+2,replacestr);//+2表示包括{}一起处理
			i=prei-1;
		}
		i++;
	}
	//处理完毕，此时该正规式中应当没有用户自定义的标识符了
	string tempRe(512,' ');
	i=0;
	while(i<re.size())
	{
		switch(re[i])
		{
		case '[':
		case ']':
			break;
		case '-':
			{
				for(char c=re[i-1]+1;c<=re[i+1];c++)
				{
					tempRe[j++]='|';
					tempRe[j++]=c;
				}
				i++;
				if(re[i+1]!=']')
					tempRe[j++]='|';
				break;
			}
		default:
			tempRe[j++]=re[i];
		}
		i++;
	}
	re=tempRe;
	re.resize(j);
}
void completRe(string &re)
{
	unsigned int i=0,j=0;
	string tempRe(512,' ');
	while(i<re.size())
	{
		char c=re[i];
		tempRe[j++]=re[i];
		if(isAlphanum(re[i]))
			c='a';
		switch(c)
		{
		case 'a':
			{			
				if(isAlphanum(re[i+1])||(re[i+1]=='('))
					tempRe[j++]='@';
				break;
			}
		case ')':
			{
				if(isAlphanum(re[i+1])||(re[i+1]=='('))
					tempRe[j++]='@';
				break;
			}
		case '*':
			{
				if(isAlphanum(re[i+1])||(re[i+1]=='('))
					tempRe[j++]='@';
				break;
			}
		}
		i++;
	}
	re=tempRe;
	re.resize(j);
}

void postFix(string &e)//将中缀转化为后缀
{
	string result=""; //返回的最终结果
	stack<char> s;    //符号栈
	e+="#$";          //$标记字符串结束
	s.push('#');
	for(int i=0;e[i]!='$';i++)
	{
		char x=e[i];
		if(isAlphanum(x))
			result+=x;
		else 
		{
			char y=s.top();
			s.pop();  
			if(x==')')//遇到了右括号：弹出栈中的所有操作符，直到遇到左括号
			{
				while(y!='(')
				{
					result+=y;
					y=s.top();
					s.pop();
				}
			}
			else
			{
				while(isp(y)<=icp(x))//操作符的栈内优先级高于等于当前扫描的操作符的入栈优先级，该操作符应出栈
				{
					result+=y;
					y=s.top();
					s.pop();				
				}
				s.push(y);//多出了一次栈
				s.push(x);//加入新的操作符
			}
		}
	}
	e=result;
}
int isp(char x)//栈内优先级
{
	if(x=='*')
		return 1;
	else if((x=='|')||(x=='@'))
		return 2;
	else        //'('或'#'
		return 4;
}
int icp(char x)//入栈优先级
{
	if(x=='(')
		return 0;
	else if(x=='*')
		return 1;
	else if((x=='|')||(x=='@'))
		return 2;
	else       //'#'
		return 3;
}
bool isAlphanum(char x)
{
	if((x>='a'&&x<='z')||(x>='A'&&x<='Z')||(x>='0'&&x<='9'))
		return true;
	else
		return false;
}
void ConstructNFA()
{
	cout<<"construct a small NFA for every RE and join them as a big NFA."<<endl;
//	cout<<"Regular expresion\t\tAction"<<endl;
	int i,j;
	int index=0;
	for(i=0;i<reTable.size();i++)
	{
		string re=reTable[i];
	//	cout<<re;
		NFA nfa1;
		if(re[0]=='\"')    //符号
		{
			Node_Edge *pStartState=new Node_Edge(++stateID);
			nfa1.push_back(pStartState);
			for(j=1;j<re.size()-1;j++)
			{
				Node_Edge *pNode=new Node_Edge(++stateID);
				nfa1[nfa1.size()-1]->addTransition(re[j],pNode);
				nfa1.push_back(pNode);
				symbolTable.insert((int)re[j]);	 		
			}
		}
		else if(re=="{id}") //是标识符
		{
			Node_Edge *pStartState=new Node_Edge(++stateID);
			nfa1.push_back(pStartState);
			Node_Edge *pNode=new Node_Edge(++stateID);
			nfa1.push_back(pNode);
			for(j=65;j<91;j++)
			{
				nfa1[0]->addTransition(j,pNode);
				nfa1[0]->addTransition(j+32,pNode);
				nfa1[1]->addTransition(j,pNode);
				nfa1[1]->addTransition(j+32,pNode);
				symbolTable.insert(j);
				symbolTable.insert(j+32);
			}
			for(j=48;j<58;j++)
			{
				nfa1[1]->addTransition(j,pNode);
				symbolTable.insert(j);
			}
		}
		else if (re[0]!='\"' && re[0]!='{')
		{
			Node_Edge *pStartState=new Node_Edge(++stateID);
			nfa1.push_back(pStartState);
			for(j=0;j<re.size();j++)
			{
				Node_Edge *pNode=new Node_Edge(++stateID);
				nfa1[nfa1.size()-1]->addTransition(re[j],pNode);
				nfa1.push_back(pNode);
				symbolTable.insert((int)re[j]);	 		
			}

		}
		else     //一般情况用Thompson构造法，当然标识符也可以用这个方法构造，但是会花费很长的构造时间
		{
			//预处理
			replaceRe(re,id2reTable); //替换用户自定义的式子
			completRe(re); //插入连接运算符,这里用@表示	
			postFix(re);   //中缀表达式转为后缀表达式
			Thompson(re,nfa1);
		}
		//记录终态
		pair<int,int> pi;   
		pi.first=(*(nfa1.end()-1))->state; 
		pi.second=index++;        //终态对应的正规表达式
		nfaTeminal.insert(pi);    //终态表，可以根据这张表，找到终态对应的动作			
		
		Join(nfa1); //合并两个nfa
	//	cout<<"\t\ta small NFA constructed."<<endl;  
	}//end for
	Node_Edge *pStartState=new Node_Edge(0);     //加入一个初态，为0
	pStartState->addTransition(EPSLONG,nfa[0]);
	nfa.push_front(pStartState);
	cout<<"A big NFA constructed"<<endl;
}

void Thompson(string &re,NFA &tnfa)  //采用汤普森算法构造NFA
{
	for(int i=0; i<re.size();i++)
	{
		char c=re[i];
		switch(c)
		{
		case '|':            //或操作
			{
				NFA A, B;    //需要两个操作符 
				if(!Pop(B)||!Pop(A)) 
				{
					cout<<"line "<<linenum<<":Produce NFA failed"<<endl;
					exit(1);
				}
				//化简处理，这样做主要是为了减少大量的EPSLONG边，试试看
				Node_Edge *pEndState=new Node_Edge(++stateID);
				A[A.size()-1]->addTransition(EPSLONG, pEndState);
				B[B.size()-1]->addTransition(EPSLONG, pEndState);
				B.push_back(pEndState);
				A[0]->addTransition(EPSLONG,B[0]);//???
				A.insert(A.end(), B.begin(), B.end());
				nfaStack.push(A);	
				break;
			} 
		case '@':             //连接操作
			{
				NFA A, B;      //需要两个操作符 
				if(!Pop(B)||!Pop(A))
				{
					cout<<"line "<<linenum<<":Produce NFA failed"<<endl;
					exit(1);
				}
				A[A.size()-1]->addTransition(EPSLONG, B[0]);//A的末态增加EPSLONG边到B的初态
				A.insert(A.end(), B.begin(), B.end());//将B的内容插入到A中
				nfaStack.push(A);                     //将A压入栈中
				break;
			}
		case '*':             //闭包操作
			{
				NFA A;       //需要一个操作符
				if(!Pop(A))
				{
					cout<<"line "<<linenum<<":Produce NFA failed"<<endl;
					exit(1);
				}
				Node_Edge *pStartState=new Node_Edge(++stateID);
				Node_Edge *pEndState=new Node_Edge(++stateID);
				pStartState->addTransition(EPSLONG, pEndState);
				pStartState->addTransition(EPSLONG, A[0]);  //从pStartState增加EPSILON边到A初态
				A[A.size()-1]->addTransition(EPSLONG, pEndState);   //从A末态增加EPSILON边到pEndState
				A[A.size()-1]->addTransition(EPSLONG, A[0]);     //A从末态增加EPSILON边到A初态     
				//加入新增的两个状态
				A.push_back(pEndState);
				A.push_front(pStartState);
				nfaStack.push(A);
				break;
			}
		default:
			//如果是操作数，直接压入栈中。注意压入栈中的是一个具有两个节点的简单NFA
			Push(c);
			symbolTable.insert((int)c);
		}
	}//end for
	if(!Pop(tnfa))
	{
		cout<<"line "<<linenum<<":Produce NFA failed"<<endl;
		exit(1);
	}
}

void Join(NFA &tnfa) //合并两个NFA
{
	if(nfa.empty())
	{
		nfa=tnfa;
	}
	else
	{
		nfa[0]->addTransition(EPSLONG,tnfa[0]);
        nfa.insert(nfa.end(), tnfa.begin(), tnfa.end());
	}
}

bool Pop(NFA &tnfa)     //NFA栈的出栈操作
{
	if(nfaStack.empty())
		return false;
	tnfa=nfaStack.top();
	nfaStack.pop();
	return true;
}
void Push(char chInput)  //NFA栈的入栈操作
{
    Node_Edge *s0=new Node_Edge(++stateID);
    Node_Edge *s1=new Node_Edge(++stateID);
    s0->addTransition((int)chInput, s1);
    NFA simpleNfa;
    simpleNfa.push_back(s0);
    simpleNfa.push_back(s1);
    nfaStack.push(simpleNfa);
}

void NFAtoDFA()    //将NFA转化为相应的DFA
{
	cout<<"NFA is exchanged to DFA,please wait a momont"<<endl;
	set<int> T; 
	T.insert(0);
	pair<set<int>,int> pi;
	pi.first=EpsilonClosure(T);  //初始化为EpsilonClosure{s0},为dStates中的唯一状态
	if(pi.first.empty())  //若为空，则出错
	{
		cout<<"NFA is NULL,please check the re"<<endl;
		exit(1);
	}
	pi.second=0;  
	dStates.insert(pi);    
	map<set<int>,int>::iterator m_iter=dStates.begin();	
	//初始化结束,下面开始用子集构造法构造DFA
    do     
	{
		T=m_iter->first;	
		m_iter->second=(-m_iter->second);   //大于等于0表示标记T
		Node_Edge *A=new Node_Edge(m_iter->second);//增加新状态
		dfa.push_back(A);       //插入新的状态
		int pos=dfaIsTerminated(T); //返回的是dfa终态对应的动作表达式编号
		if(pos>=0) //修改dfa的终态表
		{
			pair<int,int> pi;
			pi.first=m_iter->second;//dfa状态号
			pi.second=pos;//正规式编号
			dfaTeminal.insert(pi);
		}
		for(set<int>::iterator s_iter=symbolTable.begin();s_iter!=symbolTable.end();s_iter++) //针对每个输入符号
		{
			int a=*s_iter;                         //输入符号
			set<int> U=EpsilonClosure(MoveTo(T,a));  //经过a到达状态的EpsilonClosure
			if(U.size()!=0)  //为空表示无边
			{
				if(dStates.find(U)==dStates.end()) //没有在已有状态中
				{
					pi.first=U;
					pi.second=-dStates.size();
					dStates.insert(pi);            //加进去
				}
				m_iter=dStates.find(U);
				int state=m_iter->second;  //获得状态值
				if(state<0)           //可能是已标记的状态也可能是没有被标记的状态
					state=-state;
				A->addTransition(a,state);	  //增加转换
			}				
		}//end for		
	}while(GetUnmarked(m_iter)); //是否还存在未被标记的状态
	cout<<"NFA to DFA succeed"<<endl;
}

int dfaIsTerminated(set<int> &T)//判断一个状态集是不是含终态
{
	for(set<int>::iterator s_iter=T.begin();s_iter!=T.end();s_iter++)
	{
		map<int,int>::iterator m_iter=nfaTeminal.find(*s_iter);
		if(m_iter!=nfaTeminal.end())
		{
			return m_iter->second;
		}
	}
	return -1;
}
int mdfaIsTerminated(set<int> &T){ //判断最小化的一个状态集是不是含终态
	for(set<int>::iterator s_iter=T.begin();s_iter!=T.end();s_iter++)
	{
		map<int,int>::iterator m_iter=dfaTeminal.find(*s_iter);
		if(m_iter!=dfaTeminal.end())
		{
			return m_iter->second;
		}
	}
	return -1;

}
bool GetUnmarked(map<set<int>,int>::iterator &m_iter) //判断是否还有没有标记的状态
{
	for(m_iter=dStates.begin();m_iter!=dStates.end();m_iter++)
	{
		if(m_iter->second<0)  //找没有被标记的状态，状态值小于0就是没有被标记的
			return true;
	}//end for
	return false;
}
set<int> EpsilonClosure(set<int> &T)//求EpsilonClosure闭包
{
	set<int> result=T;  //返回的结果,初始化为EpsilonClosure(T)
	stack<int> state;   //状态栈
    for(set<int>::iterator p=T.begin();p!=T.end();p++) //压入集合T的所有状态
		state.push(*p);
	while(!state.empty())
	{
		int t=state.top(); //弹出栈顶元素
		state.pop();
		multimap<int,int> table=GetTable(t); //根据状态获得状态转换表
		for(multimap<int,int>::iterator iter=table.begin();iter!=table.end();iter++)
		{
			int u=iter->first;             
			int v=iter->second;
			if((u==EPSLONG)&&(result.find(v)==result.end()))  //EPSLONG边，并且不在集合result中
			{
				result.insert(v);   //加入结果中
				state.push(v);		//压入栈中		
			}
		}//end for
	}//end while
	return result;
}

multimap<int,int> GetTable(int state)   //获得状态的映射表地址
{
	NFA::iterator iter;
	for(iter=nfa.begin();iter!=nfa.end();iter++)
	{
		int v=(*iter)->state;
		if(state==v)
			break;
	}
	return (*iter)->transTable;
}

multimap<int,int> GetTable1(int state)   //获得状态的映射表地址
{
	NFA::iterator iter;
	for(iter=dfa.begin();iter!=dfa.end();iter++)
	{
		int v=(*iter)->state;
		if(state==v)
			break;
	}
	return (*iter)->transTable;
}

set<int> MoveTo(set<int> &T,int a)  //求MoveTo(T,a)
{
	set<int> result;    
	stack<int> state;   //状态栈
    for(set<int>::iterator p=T.begin();p!=T.end();p++) //压入集合T的所有状态
		state.push(*p);	
	while(!state.empty())
	{
		int t=state.top(); //弹出栈顶元素
		state.pop();
		multimap<int,int> table=GetTable(t); //根据状态获得状态转换表
		for(multimap<int,int>::iterator iter=table.begin();iter!=table.end();iter++)
		{
			if(iter->first==a)  //匹配
			{
				int u=iter->second;
				if(result.find(u)==result.end())//不在结果中，加进去
				{
					result.insert(u);   //加入结果中
				}
			}
		}//end for
	}//end while
	return result;
}

void GenerateCode(DFA &dfa)
{
	cout<<"Begin to generate code..."<<endl;
    ofile<<"int analysis(const char *yytext,int n);\n";
	//ofile<<"bool isAlphanum(char x);\n";
	ofile<<"int analysis(const char *yytext,int n)\n";
	ofile<<"{\n";
	ofile<<"\tint state="<<getMiniStartState()<<";\n";   //初始化
	ofile<<"\tint N=n+1;//N表示串长加1,为与状态数保持一致。\n";
	ofile<<"\tfor(int i=0;i<N;i++)\n";
	ofile<<"\t{\n";
    ofile<<"\t\tswitch(state)\n";
	ofile<<"\t\t{\n";
	//下面开始进入构造阶段
	for(unsigned int j=0;j<dfa.size();j++)
	{
		int i=dfa[j]->state;
		ofile<<"\t\tcase "<<i<<":\n";
		ofile<<"\t\t\t{\n";
		
        map<int,int>::iterator m_iter=mdfaTeminal.find(i);
		if(m_iter!=mdfaTeminal.end()) 
		{
			ofile<<"\t\t\t\tif(i==N-1)\n";
			ofile<<"\t\t\t\t{\n";
			//此处应当打印出识别出该终态时相应的动作。
			size_t length=actionTable[m_iter->second].size();
			ofile<<"\t\t\t\t\t"<<actionTable[m_iter->second].substr(1,length-2)
				<<endl;
			ofile<<"\t\t\t\t}\n";
		}
	/*	multimap<int,int> table=dfa[j]->transTable;
		int state;
		bool flag=FindSameState(table,state);
		
		if(flag&&i!=0) //打印程序优化，这里是对ID重复部分用else语句优化，这样可以大量减少if语句
		{
			for(multimap<int,int>::iterator iter=table.begin();iter!=table.end();iter++)
			{
				if(iter->second!=state)
				{
					ofile<<"\t\t\t\tif(yytext[i]=='"<<(char)iter->first<<"')\n";
					ofile<<"\t\t\t\t{\n";
					ofile<<"\t\t\t\t\tstate="<<iter->second<<";\n";
					ofile<<"\t\t\t\t\tbreak;\n";
					ofile<<"\t\t\t\t}\n";	
				}
			}
			ofile<<"\t\t\t\telse if(isAlphanum(yytext[i]))\n";
			ofile<<"\t\t\t\t{\n";
			ofile<<"\t\t\t\t\tstate="<<state<<";\n";
			ofile<<"\t\t\t\t\tbreak;\n";
			ofile<<"\t\t\t\t}\n";
		}
		else     //一般状况，不需要优化
		{*/
			for(multimap<int,int>::iterator iter=dfa[j]->transTable.begin();iter!=dfa[j]->transTable.end();iter++)
			{
				ofile<<"\t\t\t\tif(yytext[i]=='"<<(char)iter->first<<"')\n";
				ofile<<"\t\t\t\t{\n";
				ofile<<"\t\t\t\t\tstate="<<iter->second<<";\n";
				ofile<<"\t\t\t\t\tbreak;\n";
				ofile<<"\t\t\t\t}\n";	
			}
		//}
		ofile<<"\t\t\t\telse\n";
		ofile<<"\t\t\t\t{\n";
		ofile<<"\t\t\t\t\treturn ERROR;\n";
		ofile<<"\t\t\t\t}\n";
		ofile<<"\t\t\t}\n";
	}
	ofile<<"\t\t}\n";
	ofile<<"\t}\n";
	ofile<<"}\n";
	//输出判断是不是字母和数字的函数
	/*ofile<<"bool isAlphanum(char x)\n";
	ofile<<"{\n";
	ofile<<"\tif((x>='a'&&x<='z')||(x>='A'&&x<='Z')||(x>='0'&&x<='9'))\n";
	ofile<<"\t\treturn true;\n";
	ofile<<"\telse\n";
	ofile<<"\t\treturn false;\n";
	ofile<<"}\n";*/
	cout<<"Code is generated succeed."<<endl;
}

bool FindSameState(multimap<int,int> &table,int &s)
{
	int V[500]={0};
	for(multimap<int,int>::iterator m_iter=table.begin();m_iter!=table.end();m_iter++)
	{
		V[m_iter->second]++;
		if(V[m_iter->second]>=11)
		{
			s=m_iter->second;
			return true;
		}
	}
	return false;
}
void AnalysisProgramSegment()
{
	string str;
	while(!ifile.eof())
	{
		getline(ifile,str);
		ofile<<str<<endl;
	}	
	ofile.close();
}

void miniDFA(){     //最小化DFA
	//先分成两个集合（非终结态，终结态）
	cout<<"MiniDfa start.Please wait a monment...."<<endl;
	bool flag=true;
	int i,j;
	set<int> Nonter;
	set<int> Ter;
	vector<int> mark;
	
	for (i=0;i<dfa.size();i++)
	{
		 map<int,int>::iterator m_iter=dfaTeminal.find(dfa[i]->state);
         if(m_iter!=dfaTeminal.end())
			 Ter.insert(dfa[i]->state);
		 else
			 Nonter.insert(dfa[i]->state);
	}
    mdfa.push_back(Nonter);
	mdfa.push_back(Ter);
	//遍历,进行划分
	while(flag){	
		for (i=0;i<mdfa.size();i++)      //对每一个集合进行扫描
		{
			set<int> s;   //用于临时存放被划分的状态
			for(set<int>::iterator s_iter=mdfa[i].begin();s_iter!=mdfa[i].end();s_iter++)  //对每个集合中的元素进行扫描
			{
                
				 multimap<int,int> table=GetTable1(*s_iter);  //根据状态得到映射表
				 if(s_iter==mdfa[i].begin()){    //若它是集合set中的第一个元素的话，将他的触边都记下来
					 for(set<int>::iterator s_iter1=symbolTable.begin();s_iter1!=symbolTable.end();s_iter1++)
					 {
						 if (table.find(*s_iter1)==table.end())
						 {
							 mark.push_back(-1);    //-1代表不存在触边
						 }
						 else{
							 for(multimap<int,int>::iterator mm_iter1=table.begin();mm_iter1!=table.end();mm_iter1++)
							 {
								 if(mm_iter1->first==*s_iter1)
								 {
									 int temp=mm_iter1->second;
									 int number=SearchCollectNumber(temp);   //用于找该状态属于哪个集合
									 mark.push_back(number);    //将集合的号加入mark中
									 
									 break;
								 }
								 else
									 continue;
								 
							}

						 }
						 
					 }
				 }
				 else{      //若不是集合中第一个元素，则先生成一个mark1，mark1和mark比较，若相等则不分
                     vector<int> mark1;
					 for(set<int>::iterator s_iter2=symbolTable.begin();s_iter2!=symbolTable.end();s_iter2++)
					 {
						 if (table.find(*s_iter2)==table.end())
						 {
							 mark1.push_back(-1);
						 }
						 else
						 {
							 for(multimap<int,int>::iterator mm_iter=table.begin();mm_iter!=table.end();mm_iter++)
							 {
								 if(mm_iter->first==*s_iter2)
								 {
									 int temp=mm_iter->second;
									 int number=SearchCollectNumber(temp);   //用于找该状态属于哪个集合
									 mark1.push_back(number);    //将集合的号加入mark中
									 
									 break;
								 }
								 else
									 continue;
								 
							 }
						 }
						 
					}
					 //比较mark 和mark1
					for(vector<int>::iterator v_iter1=mark.begin(),v_iter2=mark1.begin();v_iter2!=mark1.end(); ++v_iter2,++v_iter1){
							 if( (*v_iter1) != (*v_iter2)){
								 
								 s.insert(*s_iter);   //若不相等的话则将该状态号加入到临时集合s中
								 break;    //跳出for
								 
							 }
					}

                 }//end else

			}//end for 
			if (s.size()!=0)    //根据s中元素的个数，判断s是否要正是加入mdfa中
			{
				//根据s中的元素将mdfa中的元素删除
				for (set<int>::iterator v_iter3=s.begin();v_iter3!=s.end();v_iter3++)
				{
					for (set<int>::iterator s_iter4=mdfa[i].begin();s_iter4!=mdfa[i].end();s_iter4++)
					{
						if (*s_iter4==*v_iter3)
						{
							mdfa[i].erase(s_iter4);
						    break;
						}
					}
				}
				
				mdfa.push_back(s);
				break;   //跳出，重新扫描
			}
		}//end for 
		if (i==mdfa.size())   //若不再有划分 ，则结束划分
		{
			flag=false;      
		}
	}//end while

	 //到这划分完成  大功告成 呵呵呵呵呵  继续后续工作  

	//将mdfa转化为标准的
	for (i=0;i<mdfa.size();i++)   //循环
	{
        Node_Edge *node=new Node_Edge(stateMiniDFA++);
        set<int>::iterator ss_iter=mdfa[i].begin();
	    multimap<int,int> table=GetTable1(*ss_iter);  //根据状态得到映射表
		//记录触边
		//vector<int>  v;//用于记录各条触边
			for(multimap<int,int>::iterator mm_iter=table.begin();mm_iter!=table.end();mm_iter++)
			{
					int edge=mm_iter->first;
					int state=mm_iter->second;
					int number=SearchCollectNumber(state);   //用于找该状态属于哪个集合
					node->addTransition(edge,number);
				
			}
		mdfa1.push_back(node); //加入mdfa中

	}//转换完成


	//处理终结符和非终结符  终结符含有dfaTermanate中的元素
	for (j=0;j<mdfa.size();j++)
	{
       int pos=mdfaIsTerminated(mdfa[j]);
	   if (pos>=0)
	   {
           pair<int,int> pi;
		   pi.first=j;
		   pi.second=pos;
		   mdfaTeminal.insert(pi);
	   }

	}

	cout<<"MiniDFA succeed."<<endl;

}//end

int SearchCollectNumber(int number)
{
	for (int i=0;i<mdfa.size();i++)
	{
		for (set<int>::iterator s_iter=mdfa[i].begin();s_iter!=mdfa[i].end();s_iter++)
		{
			if(number==(*s_iter)){
				return i;
			}

		}
	}
}

int getMiniStartState(){
	//找起始结点  dfa中dfa[0]就是起始节点
	int state=dfa[0]->state;
	for (int j=0;j<mdfa.size();j++)
	{
        //遍历整个队列
		for (set<int>::iterator s_iter=mdfa[j].begin();s_iter!=mdfa[j].end();s_iter++)
		{
            if (*s_iter==state)
            {
				return j;
            }
		}
	}


}