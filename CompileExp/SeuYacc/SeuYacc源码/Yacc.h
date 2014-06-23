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

//�������ʽ
struct Producer                
{
	int left;
	vector<int> right;
};
vector<Producer> producerSet;     //�����еĲ���ʽ

//������Ŀ����
class Item{                    //������Ŀ��
public:
	int pn;                    //�洢����ʽ�ı��
	int pos;                   //�洢����Ŀ���ƽ�λ��
	set<int> searchSym;        //�洢����Ŀ��������
	Item(int Pn,int Pos,set<int> &S);     
	int getNextSym();          //������һ����Ҫ�ƽ��ķ��ţ�����Ѿ���������򷵻�-1����ʾ�ɹ�Լ
	int getNNextSym();         //������һ��Ҫ�ƽ��ķ��ţ�����հ������м���������ʱʹ��
	void move();               //�ƽ�һ������
	bool isEnd();              //�Ƿ��Ҳ��Ѿ�ȫ���ƽ�
	bool nextIsEnd();          //�Ҳ��Ƿ񵽴����һ�����ţ���հ�����ʱ��
	bool IsMerge(Item &it);    //�ж�������Ŀ�ǲ�����Ҫ�ϲ�
	void Merge(Item &it);       //�ϲ�������Ŀ
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

bool operator<(const Item &it1,const Item &it2) //����С�ں���������ܹؼ���
{
	if(it1.pn==it2.pn)
	{
		if(it1.pos==it2.pos)
			return it1.searchSym<it2.searchSym;
		else return it1.pos<it2.pos;
	}
	else return it1.pn<it2.pn;
}
//������Ŀ���ඨ�����

//����ȫ�ֱ��� ʹ��map��setʱҪע�����ǵĴ洢�ṹ�Ƕ�����
ifstream fin;
ofstream fout;
ofstream lout;                    //�����־
int priority=0;                   //����������ȼ�����ֵԽ�����ȼ�Խ��
int state=0;                      //��Ŀ����״̬���
vector<vector<int> > actionTable; //�洢LR���������еڶ�ά��С������֪
vector<vector<int> > LAactionTable; //�洢��ʱLALR���������ڴ��빹�죬���еڶ�ά��С������֪
vector<vector<int> > LAactionTableLast; //�洢����LALR���������еڶ�ά��С������֪
map<int,int> tActionTable;        //�洢�ս�����������ӳ��
map<int,int> ntActionTable;       //�洢���ս�����������ӳ��
map<string,int> terminalTable;    //���ֱ��뵽�ս����ӳ��
map<string,int> nterminalTable;   //���ֱ��뵽���ս����ӳ��
map<int,int> preTable;            //��������ȼ���
map<int,int> preP;                //����ʽ���ȼ������������в���ʽ�����ȼ���ֻ��������ͻ��
map<int,set<int> > rhSet;         //�󲿵��Ҳ���һ�ķ����ż���ӳ��
map<int,set<int> > lpSet;         //�󲿵�����ʽ����ӳ��
map<int,set<int> > firstSet;      //����̬���׷���
set<int> leftTable;               //�������������
set<int> rightTable;              //�ҽ�����������
map<int,string>  semanticTable;   //�����嶯��

//�ļ���������
void Init();                    //��ʼ������
void ReadDeclaration();         //�������Σ���Ҫ�Ƕ�%token %left %right
void ReadRule();                //������Σ���Ҫ�Ƕ�����ʽ
void ReadProducer();            //����������һ���Ĳ���ʽ

//����LR��Ŀ�����õ���һЩ����
void CreateLR();
void rightHeadSet();             //�����ͬ�󲿵��Ҳ���
void getFirstSet();              //�����з���̬�����׷���
void CalIndex();                 //������̬�������̬���ڷ������е�λ��    
void CLOSURE(set<Item> &I);      //����Ŀ�ıհ�
set<int> FIRST(int left,set<int> right);  //�����̬���׷���
 //LR(1) ��LALR��ӳ��
void LRtoLALR(map<set<Item>,int> &itemsetTable);//����LALR(1)��Ŀ���弰������
multimap<set<Item>,int> clearsym(map<set<Item>,int> &itemsetTable);//ȥ��������,���ԱȽ��Ƿ�ͬ��
void merge(int,int,map<set<Item>,int> &LALRsetTable,map<set<Item>,int> &itemsetTable);//�ϲ�ͬ��״̬��������
void ChangeState(map< int, set<int> >);//�滻�ϲ����״̬
//һЩ��ӡ����                
void PrintActionTable();         //��ӡLR(1)������
void PrintLAActionTable();       //��ӡ��ʱLALR(1)������
void PrintLAActionTableLast();       //��ӡ����LALR(1)������
void PrintFirstSet();            //��ӡ���ս�����׷���
void PrintTable();               //��ӡһЩ��Ҫ��Ϣ��������ʹ��
void Print();                    //��ӡ�ܿس���
void PrintHeader();              //��ӡһЩ�����ͺ���
void PrintSymTable();            //��ӡ���ű���غ���
void PrintGetToken();            //��ӡ��token����
void PrintInit();                //��ӡ��ʼ�����������ս���ͷ��ս��ӳ�䵽���е�������λ��
void PrintMain();                //��ӡ���ɳ����������
void PrintParser();              //��ӡ�ܿس���
void PrintSemantic();            //��ӡ������Ƴ���
//���Ժ���
void displayItem(Item &item);
void displayItemSet(set<Item> &itemSet,int s);