#include<map>
#include <set>
using namespace std;
//using   namespace   stdext;
class Node_Edge{   //״̬�ڵ�	
public:
	int state;                      //�ڵ��״ֵ̬
	multimap<int,int>  transTable;  //�洢״̬ת���ı��
	Node_Edge(int s);
	void addTransition(int c,Node_Edge *state);
	void addTransition(int c,int state);
	void print();
};
Node_Edge::Node_Edge(int s)
{
	state=s;
}
void Node_Edge::addTransition(int c,Node_Edge *a) //����ת��
{
	pair<int,int> pi;
	pi.first=c;
	pi.second=a->state;
	transTable.insert(pi);
}
void Node_Edge::addTransition(int c,int state) //����ת��
{
	pair<int,int> pi;
	pi.first=c;
	pi.second=state;
	transTable.insert(pi);
}
void Node_Edge::print()   //��ӡת����
{ 
	for(multimap<int,int>::iterator iter=transTable.begin();iter!=transTable.end();iter++)
	{
		cout<<"State"<<state<<":ͨ��"<<(char)iter->first<<"�ߵ���"<<iter->second<<endl;
	}
}
//�Զ������� 
#define ERROR -2
#define EPSLONG -1
#define BEGIN 0
#define LAYER_ID 1    //��ʶ%%
#define HEADER_BEGIN 2//��ʶ%{
#define HEADER_END 3  //��ʶ%}
typedef deque<Node_Edge *> NFA;  //��˫�˶��б�ʾNFA
typedef deque<Node_Edge *> DFA;  //��˫�˶��б�ʾDFA
typedef deque<set<int> >   MDFA; //��˫�˶��б�ʾMDFA