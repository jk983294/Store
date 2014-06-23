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
//ȫ�ֱ���
ofstream ofile;      
ifstream ifile;
int linenum=0;                  //��¼�к�
int stateID=0;                  //״̬����
int stateMiniDFA=0;             //��С��dfa״̬����
set<int> symbolTable;           //����ķ��ű�,���ڴ�Ŵ��ߵĸ��ֿ���
map<string,string> id2reTable;  //�Զ����ʶ������������ʽ��ӳ��
map<int,int> nfaTeminal;        //NFA��̬��Ӧ�Ķ�����
map<int,int> dfaTeminal;        //DFA��̬��Ӧ�Ķ�����
map<int,int> mdfaTeminal;       //��С��DFA��̬��Ӧ�Ķ�����
//map<int> minidfaTeminal;        //��¼��С�����ս�̫
vector<string> actionTable;     //������
vector<string> reTable;         //������ʽ��
stack<NFA> nfaStack;            //NFAջ����������ʽ����NFAʱҪ�õ�
map<set<int>,int> dStates;      //DFA�ڵ��״̬���
NFA nfa;                        //���յ�NFA
DFA dfa;                        //���յ�DFA
MDFA mdfa;                      //��С��DFA
DFA mdfa1;                      //��С����׼��DFA
time_t begin,end;               //����ʱ��
void Init();                     //��ʼ��,����������ļ�
void AnalysisDefineSegment();   //ɨ�趨���
void AnalysisRuleSegment();     //ɨ������
void AnalysisProgramSegment();  //���������
void ConstructNFA();             //����NFA

int  CheckSpecialmark(char c);     //������ż��

void  replaceRe(string &re,map<string,string> &id2reTable);//�滻�ӱ��ʽ
void  completRe(string &re);//�������ʽ �����ӷ�@
void  postFix(string &re);//��׺���׺
int isp(char x);//ջ�����ȼ�
int icp(char x);//��ջ���ȼ�
bool isAlphanum(char x);//�ж��Ƿ�Ϊ���ֻ���ĸ

void Thompson(string &re,NFA &tnfa); //Thompson���취
void Join(NFA &tnfa);     //�ϲ�����NFA
bool Pop(NFA &tnfa);      //NFAջ�ĳ�ջ����
void Push(char chInput);  //NFAջ����ջ����

void NFAtoDFA();          //��NFAת��Ϊ��Ӧ��DFA
set<int> EpsilonClosure(set<int> &T); //��EpsilonClosure�հ�
set<int> MoveTo(set<int> &T,int a);  //��MoveTo(T,a)
bool GetUnmarked(map<set<int>,int>::iterator &m_iter); //���δ�����DFA��״̬
multimap<int,int> GetTable(int state);   //��nfa�л��״̬��ӳ���
multimap<int,int> GetTable1(int state);   //��dfa�л��״̬��ӳ���
int dfaIsTerminated(set<int> &T);   //�ж�dfa��һ��״̬���ǲ�����̬״̬��
int mdfaIsTerminated(set<int> &T);  //�ж���С��dfaһ��״̬���ǲ�����̬״̬��

void miniDFA();  //��С��DFA
int SearchCollectNumber(int number);  //�жϸ�״̬���ĸ������У����ؼ��Ϻ�
int getMiniStartState();  //����ȷ����С��dfa����ʼ���


void GenerateCode(DFA &dfa);//����DFA���ɴ���
bool FindSameState(multimap<int,int> &table,int &s);//���ɴ���ʱ�򻯴���
int getMiniStartState();  //����ȷ����С��dfa����ʼ���

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
	NFAtoDFA();          //NFAת��ΪDFA
	end=clock();
	cout<<"runtime: "<<double(end-begin)/CLOCKS_PER_SEC<<endl;
	begin=clock();
	miniDFA();
	end=clock();
	cout<<"runtime: "<<double(end-begin)/CLOCKS_PER_SEC<<endl;
    GenerateCode(mdfa1);   //����yylex.cpp
    AnalysisProgramSegment();
	ifile.close();
	ofile.close();
	return 0;
}

void Init()     //��ʼ������
{
	string fName;       //lex�ļ���
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
void AnalysisDefineSegment()  //ɨ�趨���
{
	cout<<"Analysis defintion segment."<<endl;
	char c=ifile.get();
	int state=CheckSpecialmark(c);
	if(state!=HEADER_BEGIN)//�жϿ�ͷ�ǲ���%{
	{
		cerr<<"line "<<linenum<<":Not begin with %{"<<endl;
		exit(1);
	}
    c=ifile.get(); 
	if(c!='\n') 
		ifile.putback(c);  
	else 
		linenum++;
	while(!ifile.eof())	//�жϵ�%}���ļ�βΪֹ����ɨ��
	{
		c=ifile.get();
		if(c=='\t') 
			continue;       //����\t�ַ��������
		else if(c=='\n') 
			linenum++; //���к������������жϴ����кš�
		else 
			if(c=='%')            //�����ܵ�%ʱ��ע���ж��ǲ����������
		{
			state=CheckSpecialmark(c);
			if(state!=HEADER_END)  //�жϽ����ǲ���%}
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
	//��ʼ�����Զ��������
	pair<string,string> pi;	
	while(!ifile.eof())
	{
		c=ifile.get();
		if(c!='%')
		{
			ifile.putback(c);//������������ַ�%���ѵ�ǰ�ַ��Ż����С�
		}
		else
		{
			state=CheckSpecialmark(c);
			if(state!=LAYER_ID)   //�û��Զ���ı�ʶ�������Ժ��д������ַ�%��
			{
				cerr<<"line "<<linenum<<":Defintion error"<<endl;
				exit(1);
			}
			break;//����������������Ĵ�����Ϊ�Ѿ����սᡣ
		}
		string id,re;
		ifile>>id>>re;
		pi.first=id;
		pi.second=re;
		id2reTable.insert(pi);	//������ʽ	
		ifile.get();
		linenum++;
	}//end while
	cout<<"Analysis defintion segment finish."<<endl;
}

void AnalysisRuleSegment() //ɨ������
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
			ifile.putback(c);//������������ַ�%���ѵ�ǰ�ַ��Ż�����
		}
		else
		{
			int state=CheckSpecialmark(c);
			if(state!=LAYER_ID)   
			{
				cerr<<"line "<<linenum<<":Defintion error"<<endl;
				exit(1);
			}
			break;//����������������Ĵ�����Ϊ�Ѿ����սᡣ
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
		actionTable.push_back(action);//���������뵽����
		reTable.push_back(re);
		c=ifile.get();
		linenum++;
	}
	cout<<"Analysis rule segment finish"<<endl;
}
void  replaceRe(string &re,map<string,string> &id2reTable)
{
	stack<int> status;//״̬ջ
	status.push(-1);
	unsigned int i=0,j=0;
	while(i<re.size())
	{
		if(re[i]=='{')
			status.push(i);//��¼�´�ʱ��λ��
		if(re[i]=='}')
		{
			int prei=status.top();
			if(prei<0) 
			{
				i++;
				continue;//��ʾ��ʱ��}������ԣ����ֱ�����
			}
			int length=i-prei-1;
			string id=re.substr(prei+1,length);
			string replacestr=id2reTable[id];
			if(replacestr.empty()) 
			{
				cout<<"replace error"<<endl;
				exit(0);
			}//��ʾ���е����ݲ��Ǳ�ʶ�������Բ�����
			re.replace(prei,length+2,replacestr);//+2��ʾ����{}һ����
			i=prei-1;
		}
		i++;
	}
	//������ϣ���ʱ������ʽ��Ӧ��û���û��Զ���ı�ʶ����
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

void postFix(string &e)//����׺ת��Ϊ��׺
{
	string result=""; //���ص����ս��
	stack<char> s;    //����ջ
	e+="#$";          //$����ַ�������
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
			if(x==')')//�����������ţ�����ջ�е����в�������ֱ������������
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
				while(isp(y)<=icp(x))//��������ջ�����ȼ����ڵ��ڵ�ǰɨ��Ĳ���������ջ���ȼ����ò�����Ӧ��ջ
				{
					result+=y;
					y=s.top();
					s.pop();				
				}
				s.push(y);//�����һ��ջ
				s.push(x);//�����µĲ�����
			}
		}
	}
	e=result;
}
int isp(char x)//ջ�����ȼ�
{
	if(x=='*')
		return 1;
	else if((x=='|')||(x=='@'))
		return 2;
	else        //'('��'#'
		return 4;
}
int icp(char x)//��ջ���ȼ�
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
		if(re[0]=='\"')    //����
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
		else if(re=="{id}") //�Ǳ�ʶ��
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
		else     //һ�������Thompson���취����Ȼ��ʶ��Ҳ����������������죬���ǻỨ�Ѻܳ��Ĺ���ʱ��
		{
			//Ԥ����
			replaceRe(re,id2reTable); //�滻�û��Զ����ʽ��
			completRe(re); //�������������,������@��ʾ	
			postFix(re);   //��׺���ʽתΪ��׺���ʽ
			Thompson(re,nfa1);
		}
		//��¼��̬
		pair<int,int> pi;   
		pi.first=(*(nfa1.end()-1))->state; 
		pi.second=index++;        //��̬��Ӧ��������ʽ
		nfaTeminal.insert(pi);    //��̬�����Ը������ű��ҵ���̬��Ӧ�Ķ���			
		
		Join(nfa1); //�ϲ�����nfa
	//	cout<<"\t\ta small NFA constructed."<<endl;  
	}//end for
	Node_Edge *pStartState=new Node_Edge(0);     //����һ����̬��Ϊ0
	pStartState->addTransition(EPSLONG,nfa[0]);
	nfa.push_front(pStartState);
	cout<<"A big NFA constructed"<<endl;
}

void Thompson(string &re,NFA &tnfa)  //��������ɭ�㷨����NFA
{
	for(int i=0; i<re.size();i++)
	{
		char c=re[i];
		switch(c)
		{
		case '|':            //�����
			{
				NFA A, B;    //��Ҫ���������� 
				if(!Pop(B)||!Pop(A)) 
				{
					cout<<"line "<<linenum<<":Produce NFA failed"<<endl;
					exit(1);
				}
				//��������������Ҫ��Ϊ�˼��ٴ�����EPSLONG�ߣ����Կ�
				Node_Edge *pEndState=new Node_Edge(++stateID);
				A[A.size()-1]->addTransition(EPSLONG, pEndState);
				B[B.size()-1]->addTransition(EPSLONG, pEndState);
				B.push_back(pEndState);
				A[0]->addTransition(EPSLONG,B[0]);//???
				A.insert(A.end(), B.begin(), B.end());
				nfaStack.push(A);	
				break;
			} 
		case '@':             //���Ӳ���
			{
				NFA A, B;      //��Ҫ���������� 
				if(!Pop(B)||!Pop(A))
				{
					cout<<"line "<<linenum<<":Produce NFA failed"<<endl;
					exit(1);
				}
				A[A.size()-1]->addTransition(EPSLONG, B[0]);//A��ĩ̬����EPSLONG�ߵ�B�ĳ�̬
				A.insert(A.end(), B.begin(), B.end());//��B�����ݲ��뵽A��
				nfaStack.push(A);                     //��Aѹ��ջ��
				break;
			}
		case '*':             //�հ�����
			{
				NFA A;       //��Ҫһ��������
				if(!Pop(A))
				{
					cout<<"line "<<linenum<<":Produce NFA failed"<<endl;
					exit(1);
				}
				Node_Edge *pStartState=new Node_Edge(++stateID);
				Node_Edge *pEndState=new Node_Edge(++stateID);
				pStartState->addTransition(EPSLONG, pEndState);
				pStartState->addTransition(EPSLONG, A[0]);  //��pStartState����EPSILON�ߵ�A��̬
				A[A.size()-1]->addTransition(EPSLONG, pEndState);   //��Aĩ̬����EPSILON�ߵ�pEndState
				A[A.size()-1]->addTransition(EPSLONG, A[0]);     //A��ĩ̬����EPSILON�ߵ�A��̬     
				//��������������״̬
				A.push_back(pEndState);
				A.push_front(pStartState);
				nfaStack.push(A);
				break;
			}
		default:
			//����ǲ�������ֱ��ѹ��ջ�С�ע��ѹ��ջ�е���һ�����������ڵ�ļ�NFA
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

void Join(NFA &tnfa) //�ϲ�����NFA
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

bool Pop(NFA &tnfa)     //NFAջ�ĳ�ջ����
{
	if(nfaStack.empty())
		return false;
	tnfa=nfaStack.top();
	nfaStack.pop();
	return true;
}
void Push(char chInput)  //NFAջ����ջ����
{
    Node_Edge *s0=new Node_Edge(++stateID);
    Node_Edge *s1=new Node_Edge(++stateID);
    s0->addTransition((int)chInput, s1);
    NFA simpleNfa;
    simpleNfa.push_back(s0);
    simpleNfa.push_back(s1);
    nfaStack.push(simpleNfa);
}

void NFAtoDFA()    //��NFAת��Ϊ��Ӧ��DFA
{
	cout<<"NFA is exchanged to DFA,please wait a momont"<<endl;
	set<int> T; 
	T.insert(0);
	pair<set<int>,int> pi;
	pi.first=EpsilonClosure(T);  //��ʼ��ΪEpsilonClosure{s0},ΪdStates�е�Ψһ״̬
	if(pi.first.empty())  //��Ϊ�գ������
	{
		cout<<"NFA is NULL,please check the re"<<endl;
		exit(1);
	}
	pi.second=0;  
	dStates.insert(pi);    
	map<set<int>,int>::iterator m_iter=dStates.begin();	
	//��ʼ������,���濪ʼ���Ӽ����취����DFA
    do     
	{
		T=m_iter->first;	
		m_iter->second=(-m_iter->second);   //���ڵ���0��ʾ���T
		Node_Edge *A=new Node_Edge(m_iter->second);//������״̬
		dfa.push_back(A);       //�����µ�״̬
		int pos=dfaIsTerminated(T); //���ص���dfa��̬��Ӧ�Ķ������ʽ���
		if(pos>=0) //�޸�dfa����̬��
		{
			pair<int,int> pi;
			pi.first=m_iter->second;//dfa״̬��
			pi.second=pos;//����ʽ���
			dfaTeminal.insert(pi);
		}
		for(set<int>::iterator s_iter=symbolTable.begin();s_iter!=symbolTable.end();s_iter++) //���ÿ���������
		{
			int a=*s_iter;                         //�������
			set<int> U=EpsilonClosure(MoveTo(T,a));  //����a����״̬��EpsilonClosure
			if(U.size()!=0)  //Ϊ�ձ�ʾ�ޱ�
			{
				if(dStates.find(U)==dStates.end()) //û��������״̬��
				{
					pi.first=U;
					pi.second=-dStates.size();
					dStates.insert(pi);            //�ӽ�ȥ
				}
				m_iter=dStates.find(U);
				int state=m_iter->second;  //���״ֵ̬
				if(state<0)           //�������ѱ�ǵ�״̬Ҳ������û�б���ǵ�״̬
					state=-state;
				A->addTransition(a,state);	  //����ת��
			}				
		}//end for		
	}while(GetUnmarked(m_iter)); //�Ƿ񻹴���δ����ǵ�״̬
	cout<<"NFA to DFA succeed"<<endl;
}

int dfaIsTerminated(set<int> &T)//�ж�һ��״̬���ǲ��Ǻ���̬
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
int mdfaIsTerminated(set<int> &T){ //�ж���С����һ��״̬���ǲ��Ǻ���̬
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
bool GetUnmarked(map<set<int>,int>::iterator &m_iter) //�ж��Ƿ���û�б�ǵ�״̬
{
	for(m_iter=dStates.begin();m_iter!=dStates.end();m_iter++)
	{
		if(m_iter->second<0)  //��û�б���ǵ�״̬��״ֵ̬С��0����û�б���ǵ�
			return true;
	}//end for
	return false;
}
set<int> EpsilonClosure(set<int> &T)//��EpsilonClosure�հ�
{
	set<int> result=T;  //���صĽ��,��ʼ��ΪEpsilonClosure(T)
	stack<int> state;   //״̬ջ
    for(set<int>::iterator p=T.begin();p!=T.end();p++) //ѹ�뼯��T������״̬
		state.push(*p);
	while(!state.empty())
	{
		int t=state.top(); //����ջ��Ԫ��
		state.pop();
		multimap<int,int> table=GetTable(t); //����״̬���״̬ת����
		for(multimap<int,int>::iterator iter=table.begin();iter!=table.end();iter++)
		{
			int u=iter->first;             
			int v=iter->second;
			if((u==EPSLONG)&&(result.find(v)==result.end()))  //EPSLONG�ߣ����Ҳ��ڼ���result��
			{
				result.insert(v);   //��������
				state.push(v);		//ѹ��ջ��		
			}
		}//end for
	}//end while
	return result;
}

multimap<int,int> GetTable(int state)   //���״̬��ӳ����ַ
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

multimap<int,int> GetTable1(int state)   //���״̬��ӳ����ַ
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

set<int> MoveTo(set<int> &T,int a)  //��MoveTo(T,a)
{
	set<int> result;    
	stack<int> state;   //״̬ջ
    for(set<int>::iterator p=T.begin();p!=T.end();p++) //ѹ�뼯��T������״̬
		state.push(*p);	
	while(!state.empty())
	{
		int t=state.top(); //����ջ��Ԫ��
		state.pop();
		multimap<int,int> table=GetTable(t); //����״̬���״̬ת����
		for(multimap<int,int>::iterator iter=table.begin();iter!=table.end();iter++)
		{
			if(iter->first==a)  //ƥ��
			{
				int u=iter->second;
				if(result.find(u)==result.end())//���ڽ���У��ӽ�ȥ
				{
					result.insert(u);   //��������
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
	ofile<<"\tint state="<<getMiniStartState()<<";\n";   //��ʼ��
	ofile<<"\tint N=n+1;//N��ʾ������1,Ϊ��״̬������һ�¡�\n";
	ofile<<"\tfor(int i=0;i<N;i++)\n";
	ofile<<"\t{\n";
    ofile<<"\t\tswitch(state)\n";
	ofile<<"\t\t{\n";
	//���濪ʼ���빹��׶�
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
			//�˴�Ӧ����ӡ��ʶ�������̬ʱ��Ӧ�Ķ�����
			size_t length=actionTable[m_iter->second].size();
			ofile<<"\t\t\t\t\t"<<actionTable[m_iter->second].substr(1,length-2)
				<<endl;
			ofile<<"\t\t\t\t}\n";
		}
	/*	multimap<int,int> table=dfa[j]->transTable;
		int state;
		bool flag=FindSameState(table,state);
		
		if(flag&&i!=0) //��ӡ�����Ż��������Ƕ�ID�ظ�������else����Ż����������Դ�������if���
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
		else     //һ��״��������Ҫ�Ż�
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
	//����ж��ǲ�����ĸ�����ֵĺ���
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

void miniDFA(){     //��С��DFA
	//�ȷֳ��������ϣ����ս�̬���ս�̬��
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
	//����,���л���
	while(flag){	
		for (i=0;i<mdfa.size();i++)      //��ÿһ�����Ͻ���ɨ��
		{
			set<int> s;   //������ʱ��ű����ֵ�״̬
			for(set<int>::iterator s_iter=mdfa[i].begin();s_iter!=mdfa[i].end();s_iter++)  //��ÿ�������е�Ԫ�ؽ���ɨ��
			{
                
				 multimap<int,int> table=GetTable1(*s_iter);  //����״̬�õ�ӳ���
				 if(s_iter==mdfa[i].begin()){    //�����Ǽ���set�еĵ�һ��Ԫ�صĻ��������Ĵ��߶�������
					 for(set<int>::iterator s_iter1=symbolTable.begin();s_iter1!=symbolTable.end();s_iter1++)
					 {
						 if (table.find(*s_iter1)==table.end())
						 {
							 mark.push_back(-1);    //-1�������ڴ���
						 }
						 else{
							 for(multimap<int,int>::iterator mm_iter1=table.begin();mm_iter1!=table.end();mm_iter1++)
							 {
								 if(mm_iter1->first==*s_iter1)
								 {
									 int temp=mm_iter1->second;
									 int number=SearchCollectNumber(temp);   //�����Ҹ�״̬�����ĸ�����
									 mark.push_back(number);    //�����ϵĺż���mark��
									 
									 break;
								 }
								 else
									 continue;
								 
							}

						 }
						 
					 }
				 }
				 else{      //�����Ǽ����е�һ��Ԫ�أ���������һ��mark1��mark1��mark�Ƚϣ�������򲻷�
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
									 int number=SearchCollectNumber(temp);   //�����Ҹ�״̬�����ĸ�����
									 mark1.push_back(number);    //�����ϵĺż���mark��
									 
									 break;
								 }
								 else
									 continue;
								 
							 }
						 }
						 
					}
					 //�Ƚ�mark ��mark1
					for(vector<int>::iterator v_iter1=mark.begin(),v_iter2=mark1.begin();v_iter2!=mark1.end(); ++v_iter2,++v_iter1){
							 if( (*v_iter1) != (*v_iter2)){
								 
								 s.insert(*s_iter);   //������ȵĻ��򽫸�״̬�ż��뵽��ʱ����s��
								 break;    //����for
								 
							 }
					}

                 }//end else

			}//end for 
			if (s.size()!=0)    //����s��Ԫ�صĸ������ж�s�Ƿ�Ҫ���Ǽ���mdfa��
			{
				//����s�е�Ԫ�ؽ�mdfa�е�Ԫ��ɾ��
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
				break;   //����������ɨ��
			}
		}//end for 
		if (i==mdfa.size())   //�������л��� �����������
		{
			flag=false;      
		}
	}//end while

	 //���⻮�����  �󹦸�� �ǺǺǺǺ�  ������������  

	//��mdfaת��Ϊ��׼��
	for (i=0;i<mdfa.size();i++)   //ѭ��
	{
        Node_Edge *node=new Node_Edge(stateMiniDFA++);
        set<int>::iterator ss_iter=mdfa[i].begin();
	    multimap<int,int> table=GetTable1(*ss_iter);  //����״̬�õ�ӳ���
		//��¼����
		//vector<int>  v;//���ڼ�¼��������
			for(multimap<int,int>::iterator mm_iter=table.begin();mm_iter!=table.end();mm_iter++)
			{
					int edge=mm_iter->first;
					int state=mm_iter->second;
					int number=SearchCollectNumber(state);   //�����Ҹ�״̬�����ĸ�����
					node->addTransition(edge,number);
				
			}
		mdfa1.push_back(node); //����mdfa��

	}//ת�����


	//�����ս���ͷ��ս��  �ս������dfaTermanate�е�Ԫ��
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
	//����ʼ���  dfa��dfa[0]������ʼ�ڵ�
	int state=dfa[0]->state;
	for (int j=0;j<mdfa.size();j++)
	{
        //������������
		for (set<int>::iterator s_iter=mdfa[j].begin();s_iter!=mdfa[j].end();s_iter++)
		{
            if (*s_iter==state)
            {
				return j;
            }
		}
	}


}