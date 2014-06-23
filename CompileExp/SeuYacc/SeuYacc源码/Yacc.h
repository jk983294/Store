#pragma warning(disable :4786) 
#include<iostream>
#include<fstream>
#include<map>
#include<set>
#include<stack>
#include<queue>
#include<vector>
#include<string>
#define NONTERMINAL 2000
#define TERMINAL 1000
#define ERROR   32676
using namespace std;
typedef map<string,int>::iterator    M_ITER;
typedef map<int,set<int> >::iterator I_ITER;
typedef set<int>::iterator           SET_ITER;

//定义产生式
struct Producer                
{
	int left;
	vector<int> right;
};
vector<Producer> producerSet;     //存所有的产生式

//定义项目集类
class Item{                    //定义项目类
public:
	int pn;                    //存储产生式的编号
	int pos;                   //存储此项目的移进位置
	set<int> searchSym;        //存储此项目的搜索符
	Item(int Pn,int Pos,set<int> &S);     
	int getNextSym();          //返回下一个将要移进的符号，如果已经到达最后，则返回-1，表示可归约
	int getNNextSym();         //返回再一下要移进的符号，在求闭包运算中计算搜索符时使用
	void move();               //移进一个符号
	bool isEnd();              //是否右部已经全部移进
	bool nextIsEnd();          //右部是否到达最后一个符号，求闭包运算时用
	bool IsMerge(Item &it);    //判断两个项目是不是需要合并
	void Merge(Item &it);       //合并两个项目
    void operator=(const Item &it);	
};
Item::Item(int Pn,int Pos,set<int> &S)
{
	pn=Pn;
	pos=Pos;
	searchSym=S;
}
int Item::getNextSym()    
{
	if(pos!=producerSet[pn].right.size())
		return producerSet[pn].right[pos]; 
	return -1;
} 
int Item::getNNextSym() 
{ 
	if(pos!=(producerSet[pn].right.size()-1))
		return producerSet[pn].right[pos+1]; 
	return -1;
}
bool Item::isEnd()
{
	if(pos==producerSet[pn].right.size())
		return true;
	return false;
}
bool Item::nextIsEnd()
{
	if(pos==producerSet[pn].right.size()-1)
		return true;
	return false;
}
void Item::move(){pos++;}
void Item::operator=(const Item &it)
{
	this->pn=it.pn;
	this->pos=it.pos;
	this->searchSym=it.searchSym;
}
bool Item::IsMerge(Item &it) 
{
	if(this->pn==it.pn&&this->pos==it.pos)
	{
		if(this->searchSym!=it.searchSym)
			return true;
		else
			return false;
	}
	else return false;
}
void Item::Merge(Item &it)
{
	for(set<int>::iterator i=it.searchSym.begin();i!=it.searchSym.end();i++)
		this->searchSym.insert(*i);
}

bool operator<(const Item &it1,const Item &it2) //重载小于函数，这个很关键的
{
	if(it1.pn==it2.pn)
	{
		if(it1.pos==it2.pos)
			return it1.searchSym<it2.searchSym;
		else return it1.pos<it2.pos;
	}
	else return it1.pn<it2.pn;
}
//到此项目集类定义好了

//定义全局变量 使用map和set时要注意它们的存储结构是二叉树
ifstream fin;
ofstream fout;
ofstream lout;                    //输出日志
int priority=0;                   //运算符的优先级，数值越大优先级越大
int state=0;                      //项目集的状态编号
vector<vector<int> > actionTable; //存储LR动作表，其中第二维大小必须已知
vector<vector<int> > LAactionTable; //存储临时LALR动作表，用于代码构造，其中第二维大小必须已知
vector<vector<int> > LAactionTableLast; //存储最终LALR动作表，其中第二维大小必须已知
map<int,int> tActionTable;        //存储终结符到动作表的映射
map<int,int> ntActionTable;       //存储非终结符到动作表的映射
map<string,int> terminalTable;    //数字编码到终结符的映射
map<string,int> nterminalTable;   //数字编码到非终结符的映射
map<int,int> preTable;            //运算符优先级表
map<int,int> preP;                //产生式优先级，并不存所有产生式的优先级，只存会引起冲突的
map<int,set<int> > rhSet;         //左部到右部第一文法符号集的映射
map<int,set<int> > lpSet;         //左部到产生式集的映射
map<int,set<int> > firstSet;      //非终态的首符集
set<int> leftTable;               //左结合运算符集合
set<int> rightTable;              //右结合运算符集合
map<int,string>  semanticTable;   //存语义动作

//文件解析函数
void Init();                    //初始化函数
void ReadDeclaration();         //读声明段，主要是读%token %left %right
void ReadRule();                //读规则段，主要是读产生式
void ReadProducer();            //读入若干左部一样的产生式

//构建LR项目集簇用到的一些函数
void CreateLR();
void rightHeadSet();             //获得相同左部的右部集
void getFirstSet();              //求所有非终态符的首符集
void CalIndex();                 //计算终态符与非终态符在分析表中的位置    
void CLOSURE(set<Item> &I);      //求项目的闭包
set<int> FIRST(int left,set<int> right);  //求非终态的首符集
 //LR(1) 到LALR的映射
void LRtoLALR(map<set<Item>,int> &itemsetTable);//构造LALR(1)项目集族及分析表
multimap<set<Item>,int> clearsym(map<set<Item>,int> &itemsetTable);//去除搜索符,用以比较是否同心
void merge(int,int,map<set<Item>,int> &LALRsetTable,map<set<Item>,int> &itemsetTable);//合并同心状态的搜索符
void ChangeState(map< int, set<int> >);//替换合并后的状态
//一些打印函数                
void PrintActionTable();         //打印LR(1)动作表
void PrintLAActionTable();       //打印临时LALR(1)分析表
void PrintLAActionTableLast();       //打印最终LALR(1)动作表
void PrintFirstSet();            //打印非终结符的首符集
void PrintTable();               //打印一些必要信息，供调试使用
void Print();                    //打印总控程序
void PrintHeader();              //打印一些常量和函数
void PrintSymTable();            //打印符号表相关函数
void PrintGetToken();            //打印读token函数
void PrintInit();                //打印初始化函数，将终结符和非终结符映射到表中的纵坐标位置
void PrintMain();                //打印生成程序的主函数
void PrintParser();              //打印总控程序
void PrintSemantic();            //打印语义控制程序
//测试函数
void displayItem(Item &item);
void displayItemSet(set<Item> &itemSet,int s);