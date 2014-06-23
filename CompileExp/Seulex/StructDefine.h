#include<map>
#include <set>
using namespace std;
//using   namespace   stdext;
class Node_Edge{   //状态节点	
public:
	int state;                      //节点的状态值
	multimap<int,int>  transTable;  //存储状态转换的表格
	Node_Edge(int s);
	void addTransition(int c,Node_Edge *state);
	void addTransition(int c,int state);
	void print();
};
Node_Edge::Node_Edge(int s)
{
	state=s;
}
void Node_Edge::addTransition(int c,Node_Edge *a) //增加转换
{
	pair<int,int> pi;
	pi.first=c;
	pi.second=a->state;
	transTable.insert(pi);
}
void Node_Edge::addTransition(int c,int state) //增加转换
{
	pair<int,int> pi;
	pi.first=c;
	pi.second=state;
	transTable.insert(pi);
}
void Node_Edge::print()   //打印转换表
{ 
	for(multimap<int,int>::iterator iter=transTable.begin();iter!=transTable.end();iter++)
	{
		cout<<"State"<<state<<":通过"<<(char)iter->first<<"边到达"<<iter->second<<endl;
	}
}
//自定义宏变量 
#define ERROR -2
#define EPSLONG -1
#define BEGIN 0
#define LAYER_ID 1    //标识%%
#define HEADER_BEGIN 2//标识%{
#define HEADER_END 3  //标识%}
typedef deque<Node_Edge *> NFA;  //用双端队列表示NFA
typedef deque<Node_Edge *> DFA;  //用双端队列表示DFA
typedef deque<set<int> >   MDFA; //用双端队列表示MDFA