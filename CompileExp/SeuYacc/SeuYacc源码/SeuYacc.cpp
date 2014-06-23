#include "Yacc.h"
int main()
{
	Init();                     //初始化函数，打开输入输出文件 
	ReadDeclaration();          //读声明段
	ReadRule();                 //读规则段
	CreateLR();                //构建项目集同时构建对应的动作表
	Print();                   //输出函数
	return 0;
}
void Init()
{	
	string name;
	cout<<"Please enter the name of input file:";
	cin>>name;
	fin.open(name.c_str());
	fout.open("parse.h");
	lout.open("myLog.txt");
	if(fin.fail())
	{
		cout<<"error:Can not open the file"<<endl;
		exit(1);
	}
	lout<<"开始初始化..."<<endl;  //打印日志信息
	//压入拓展表达式
	Producer ap;
	ap.left=NONTERMINAL-1; //1999
    ap.right.push_back(NONTERMINAL);
	producerSet.push_back(ap);
    //插入终结符'#',放在第一位置
   	pair<string,int> pi;
	pi.first="TERMINAL";
	pi.second=TERMINAL;    //1000
	terminalTable.insert(pi);
	lout<<"产生式中压入了拓展表达式"<<endl;
	lout<<"终结符表插入结束符TERMINAL"<<endl;
	lout<<"初始化结束"<<endl;
}
void ReadDeclaration()
{
	lout<<"\n开始读入声明段，存储终结符..."<<endl;
	string str;
	bool flag=true;
	getline(fin,str);
	getline(fin,str);  //%{...%}没有内容，可以根据需要添加
	while(flag)
	{
		fin>>str;
		if(str=="%token"||str=="%type") 
		{
			char c=fin.get();
			while(c!='\n')
			{				
				fin.putback(c);
				fin>>str;		
				pair<string,int> pi;							
				pi.first=str;
				pi.second=TERMINAL+terminalTable.size();
				terminalTable.insert(pi);   //插入终结符表中
				c=fin.get();
				while(c==' '||c=='\r'||c=='\t') c=fin.get();
			}
		}
		else if(str=="%left") //左结合
		{
			priority++; //优先级递增，表示下一行定义的运算符优先级高
			char c=fin.get();
			while(c!='\n')
			{		
				fin.putback(c);
				fin>>str;
				
				pair<string,int> pi;			
				pi.first=str;
				pi.second=TERMINAL+terminalTable.size();
				terminalTable.insert(pi);        //插入终结符表
				leftTable.insert(pi.second);  //插入左运算符表中
				
				pair<int,int> pi1;
				pi1.first=pi.second;
				pi1.second=priority;
				preTable.insert(pi1);  //插入算符优先表中
				c=fin.get();
				while(c==' '||c=='\r'||c=='\t') c=fin.get();
			}
		}
		else if(str=="%right")  //右结合
		{
			priority++;
			char c=fin.get();
			while(c!='\n')
			{		
				fin.putback(c);
				fin>>str;
				
				pair<string,int> pi;			
				pi.first=str;
				pi.second=TERMINAL+terminalTable.size();
				terminalTable.insert(pi);        //插入终结符表
				rightTable.insert(pi.second);  //插入右运算符表中
				
				pair<int,int> pi1;
				pi1.first=pi.second;
				pi1.second=priority;
				preTable.insert(pi1);  //插入算符优先表中
				c=fin.get();
				while(c==' '||c=='\r'||c=='\t') c=fin.get();
			}
		}
		else if(str=="%%")
		{
			flag=false;
		}
		else
		{
			exit(1);
		}
	}	
	lout<<"声明段读入完毕"<<endl;
}
void ReadRule()
{
	lout<<"\n开始读入产生式..."<<endl;
	char c=fin.get();
	while(c!='%')
	{
		fin.putback(c);
		ReadProducer();  //读产生式
		c=fin.get();
		while(c==' '||c=='\n'||c=='\r'||c=='\t') c=fin.get();
	}
	fin.close();  //读入文件结束

	lout<<"产生式读入完毕"<<endl;
	lout<<"\n打印所有读入的信息..."<<endl;
	PrintTable();   //打印一些必要信息，调试时用到的	
}
void ReadProducer() //读入产生式，可能是多个
{
	Producer ap;//定义一个产生式
	string str;
	fin>>str;  //读入文法左符
	//如果左部是终结符则报错
	if(terminalTable.count(str))  
	{
		cout<<"error:Producer begins with teminal"<<endl;
		exit(1);
	}
	//如果是非终结符串并且不在非终结符串表中，则加进去
	if(!nterminalTable.count(str)) 
	{
		pair<string,int> pi;
		pi.first=str;
		pi.second=NONTERMINAL+nterminalTable.size();
		nterminalTable.insert(pi);
		ap.left=pi.second;
	}
	//非终结符串并且在非终结符串表中
	else
	{
		ap.left=nterminalTable[str];
	}

	char c=fin.get();
	while(c==' '||c=='\n'||c=='\r'||c=='\t') c=fin.get();
	
	if(c!=':')
	{
		cout<<"Producer formation error"<<endl;
		exit(1);
	}
	while(1)
	{
		c=fin.get();
		switch(c)
		{
		case '\'': //遇到单引号,表示遇见终结符串
			str=fin.get();		
			if(!terminalTable.count(str)) //终结符串不在表中,加进来
			{
				pair<string,int> pi;
				pi.first=str;
				pi.second=TERMINAL+terminalTable.size();
				terminalTable.insert(pi);
				ap.right.push_back(pi.second);
			}
			else
			{
				int temp=terminalTable[str];
				ap.right.push_back(temp);
				if(preTable.count(temp))  //终结符有优先级
				{
					pair<int,int> pi;
					pi.first=producerSet.size();
					pi.second=preTable[temp];
					preP.insert(pi);
				}			
			}		
			fin.get(); //读取另一个'\''
			break;
		case '|':  //一个产生读完了,开始读下一个,由于它们的左部都是一样,所以只要将右部清掉就行了
			producerSet.push_back(ap); 
			ap.right.clear();
			break;
		case ';': //这里不要忘记保存产生式
			producerSet.push_back(ap); 
			ap.right.clear();
			return;
		case '\n':
		case '\t':
		case '\r':
		case ' ':
			break;
		case '%':  //强制给产生式加优先级
			{
			while(c!=' ') c=fin.get();
			fin>>str;
			pair<int,int> pi;
			pi.first=producerSet.size();
			pi.second=preTable[terminalTable[str]];
			preP.insert(pi);
			break;
			}
		case '{':  //读语义内容
			{
				char semantic[256];
				int pos=0;
				char c=fin.get();
				while(c!='}') 
				{
					semantic[pos++]=c;
					c=fin.get();
				}
                semantic[pos]='\0';
                pair<int,string> pi;
				pi.first=producerSet.size();
				pi.second=semantic;
				semanticTable.insert(pi);
//				cout<<pi.first<<" "<<pi.second<<endl;
				break;
			}
		default:  //三种情况
			fin.putback(c);
			fin>>str;
			if(terminalTable.count(str))  //已经定义的终结符
			{
				int temp=terminalTable[str];
				ap.right.push_back(temp);
				if(preTable.count(temp))  //终结符有优先级
				{
					pair<int,int> pi;
					pi.first=producerSet.size();
					pi.second=preTable[temp];
					preP.insert(pi);
				}
			}
			else if(!nterminalTable.count(str)) //非终结符串不在表中，加进来
			{
				pair<string,int> pi;
				pi.first=str;
				pi.second=NONTERMINAL+nterminalTable.size();
				nterminalTable.insert(pi);

				ap.right.push_back(pi.second);
			}			
			else                //非终结符串在表中
			{
				ap.right.push_back(nterminalTable[str]);
			}	
		}
	}
}

void rightHeadSet()
{
	for(int i=0;i<producerSet.size();i++)
	{
		Producer ap=producerSet[i];  
		if(!rhSet.count(ap.left))  //产生式左部不在rhSet表中，加进来
		{
			pair<int,set<int> > pi;
			pi.first=ap.left;
			set<int> s;
			pi.second=s;
			rhSet.insert(pi);
			//附带初始化lpSet
			lpSet.insert(pi);
		}
		rhSet[ap.left].insert(ap.right[0]);//插入产生式右部第一个文法符号
		lpSet[ap.left].insert(i);          //附带初始化lpSet,此数据结构在求闭包时用到
	}
}
void getFirstSet()  //求所有非终结符的first集合
{	
	for(I_ITER iter=rhSet.begin();iter!=rhSet.end();iter++)
	{
		if(!firstSet.count(iter->first))//如果还没有求出来，则把它求出来
		{
			FIRST(iter->first,iter->second);
		}
	}
}
set<int> FIRST(int left,set<int> right)
{	
	set<int>  tSet;
	//添加到firstSet中
	pair<int,set<int> >pi;
	pi.first=left;
	pi.second=tSet;
	firstSet.insert(pi);
    //定义一个栈，要考虑right集合中所有元素
	stack<int> tStack;
	for(SET_ITER iter=right.begin();iter!=right.end();iter++)	
	{
		tStack.push(*iter);
	}
	while(!tStack.empty())
	{
		int first=tStack.top();
		tStack.pop();
		if(tActionTable.count(first)) //是终结符，直接添加到首符集中
		{	
			firstSet[left].insert(first);
		}
		else if(firstSet.count(first)) //非终结符的firstSet已计算出来了
		{
			I_ITER iter=firstSet.find(first);
			for(SET_ITER s_iter=iter->second.begin();s_iter!=iter->second.end();s_iter++)
			{
				firstSet[left].insert(*s_iter);
			}
		}
		else        //非终结符的firstSet还没有被计算出来了
		{
			set<int> tSet1=FIRST(first,rhSet[first]);//循环计算，直至算出
			for(SET_ITER s_iter=tSet1.begin();s_iter!=tSet1.end();s_iter++)
			{
				firstSet[left].insert(*s_iter);
			}
		}
	}
	return firstSet[left];
}
void CalIndex()
{
	int i=0;
	for(M_ITER j=terminalTable.begin();j!=terminalTable.end();j++)
	{
		pair<int,int> pi;
		pi.first=j->second;
		pi.second=i++;
		tActionTable.insert(pi);//加入终结符动作表
	}
	for(M_ITER k=nterminalTable.begin();k!=nterminalTable.end();k++)
	{
		pair<int,int> pi;
		pi.first=k->second;
		pi.second=i++;
		ntActionTable.insert(pi);//非终结符动作表
	}
}
void CreateLR()
{
	lout<<"\n开始构建LR(1)文法...\n";
	//准备工作
	CalIndex();                 //计算终态符与非终态符在分析表中的位置        
	rightHeadSet();             //获得相同左部的右部集
	getFirstSet();              //获得非终态的首符集
 
	int width=tActionTable.size()+ntActionTable.size();	 //分析表的宽度
    map< set<Item>,int> itemsetTable;  //项目集表
	set<int> S;
	S.insert(TERMINAL);                //TERMINAL表示'#'
	Item firstItem(0,0,S);            //状态0的第一个项目 	
	set<Item> I0;                     //状态0的项目集
	I0.insert(firstItem);
	CLOSURE(I0);                       //求项目的闭包
	queue<set<Item> > Q_item;
	Q_item.push(I0);  
	
	pair<set<Item>,int> pi;          //将I0放入项目集表中
	pi.first=I0;
	pi.second=state;
	itemsetTable.insert(pi);
	//下面开始求项目集簇
	while(!Q_item.empty())                  //直到所有的项目集都计算出来为止
	{        
		vector<int> v(width);         //动作表一行的内容
		for(int n=0;n<width;n++)  { v[n]=ERROR;}   //以ERROR值对vector进行初始化
		actionTable.push_back(v);   
		
		map<int, set<Item> > moveItemSet;   
		set<Item> ItemSet=Q_item.front();  
		Q_item.pop();
		int column,row;                //动作表的位置
		row=itemsetTable[ItemSet];     //行坐标已知   
		for(set<Item>::iterator i=ItemSet.begin();i!=ItemSet.end();i++)
		{
			if(i->isEnd())          //是规约项目
			{
				//向表中填规约项目的产生式号
				for(set<int>::iterator j=i->searchSym.begin();j!=i->searchSym.end();j++)
				{
					//试一下，移进与规约或移进的冲突
					column=tActionTable[*j];         //获得列坐标，即终结符在teminSet表中的位置
					if(actionTable[row][column]!=ERROR)
					{
						//无法解决，如果遇到程序会退出来
						lout<<"行"<<row<<":存在规约、规约冲突，无法解决，程序终止。"<<endl;  //打印日志
						exit(1);
					}
					actionTable[row][column]=-(i->pn);   //填动作表,规约项目用负号表示,0是接受态
				}
			}
			else                       //不是规约项目，移进一个符号
			{
				Item it=*i;
				it.move();
				if(!moveItemSet.count(i->getNextSym()))
				{
					pair<int,set<Item> > pi;
					set<Item> Se;
					pi.first=i->getNextSym();
                    pi.second=Se;
					moveItemSet.insert(pi);
				}
				moveItemSet[i->getNextSym()].insert(it);
			}
		}
		//经过move操作后的项目集
		for(map<int,set<Item> >::iterator k=moveItemSet.begin();k!=moveItemSet.end();k++)
		{
			CLOSURE(k->second);	
			if(!itemsetTable.count(k->second))     //不在项目集中要加进来的
			{
				pair<set<Item>,int> pi;       
				pi.first=k->second;			
				pi.second=++state;
				itemsetTable.insert(pi);
		
				Q_item.push(k->second);    //加到队列中		
			}	
			//开始填移近项
			column=(ntActionTable.count(k->first)?ntActionTable[k->first]:tActionTable[k->first]);
			if(actionTable[row][column]!=ERROR)  //这里只解决了移进规约冲突
			{
				lout<<"行"<<row<<":存在移进、规约冲突"<<endl;
				if(preTable[k->first]>preP[-actionTable[row][column]])
				{
					actionTable[row][column]=itemsetTable[k->second];
					lout<<"移进的优先级高，移进"<<endl;
					continue;
				}
				else
				{
					lout<<"移进的优先级低，不处理"<<endl;
					continue;
				}
			}
			actionTable[row][column]=itemsetTable[k->second];
		}
	}//end while

/*	cout<<itemsetTable.size()<<endl;
	for(map< set<Item>,int>::iterator i1=itemsetTable.begin();i1!=itemsetTable.end();i1++)
	{
		set<Item> SS=i1->first;
		displayItemSet(SS,i1->second);
	}test*/
	lout<<"LR(1)文法构建完毕\n"<<"\n开始打印动作表\n";
	PrintFirstSet();
	PrintActionTable();         //打印动作表
	lout<<"\nLR(1)映射到LALR(1)\n";
	LRtoLALR(itemsetTable);

}

void CLOSURE(set<Item> &I) //求项目集的闭包运算
{  
	queue<Item> Q_item;
	
	for(set<Item>::iterator i=I.begin();i!=I.end();i++){ Q_item.push(*i); }  //初始化，将I都压入队列中
    while(!Q_item.empty())  
	{
		Item item=Q_item.front();
		Q_item.pop();		
		I.insert(item);	
		int next=item.getNextSym();//读下一个要移进的符号
        if(next!=-1&&!tActionTable.count(next))  //不为空以及不是终结符才处理，否则这个项目处理完了
		{
			set<int> np=lpSet[next];
			int nnext=item.getNNextSym();
			for(set<int>::iterator j=np.begin();j!=np.end();j++)
			{
				set<int> Sym;
				if(nnext==-1) { Sym=item.searchSym; }
				else if(tActionTable.count(nnext))  { Sym.insert(nnext); }
				else          { Sym=firstSet[nnext]; }
				Item nitem(*j,0,Sym); //增加新项目
				if(!I.count(nitem))
				{
					Q_item.push(nitem);
				}
			}
		}//end if
	}//end while
	//简化项目集
	set<Item> result;
	for(set<Item>::iterator n=I.begin();n!=I.end();n++)
	{
		for(set<Item>::iterator m=I.begin();m!=I.end();m++)
		{
			if(!n->IsMerge(*m)) continue; //不需要合并
			n->Merge(*m);                  //合并   

		}
		if(!result.count(*n)) 
			result.insert(*n);
	}			
	I=result;
}
//LR->LALR部分
void LRtoLALR(map<set<Item>,int> &itemsetTable)
{
	lout<<"开始合并同心状态\n";
	map< set<Item>,int> LALRsetTable;//最终LALR(1)项目集族
    map< int, set<int> > core;//记录同心的各状态,关键字是第一个加入状态，值是后来的与之同心的状态
	map< set<Item>,int> temp;//临时记录各状态
	multimap<set<Item>,int> itemsetTable1;//消去搜索符后的项目集族
	itemsetTable1=clearsym(itemsetTable);
	for(multimap<set<Item>,int>::iterator i=itemsetTable1.begin();i!=itemsetTable1.end();i++)
		if(!temp.count(i->first))
		{
			pair<set<Item>,int> pi;
			pi.first=i->first;
			pi.second=i->second;
			temp.insert(pi);
			for(map<set<Item>,int>::iterator j=itemsetTable.begin();j!=itemsetTable.end();j++)
			{
				if(j->second==pi.second){
					pi.first=j->first;
					break;
				}
			}
			pair<int, set<int> > pi1;
			pi1.first=i->second;
			core.insert(pi1);
			LAactionTable.push_back(actionTable[pi.second]);
            LALRsetTable.insert(pi);
		}
		else
		{
			merge(temp[i->first],i->second,LALRsetTable,itemsetTable);
			if(!core.count(temp[i->first]))//同心状态未记录,新建pair插入
			{
				pair<int, set<int> > pi1;
				pi1.first=temp[i->first];
				pi1.second.insert(i->second);
				core.insert(pi1);
			}
			else//同心状态已记录，直接添加到集合
			{
				core[temp[i->first]].insert(i->second);
			}
		}//到此同心状态完毕
   lout<<"同心状态合并完毕，开始构建分析表\n";
  /* cout<<core.size()<<endl;//输出找到的通同心状态集合
   for(map< int,set<int> >::iterator i2=core.begin();i2!=core.end();i2++)
	{
		set<int> co=i2->second;
		cout<<"{"<<i2->first;
		for(set<int>::iterator i3=co.begin();i3!=co.end();i3++)
			cout<<","<<*i3;
		cout<<"}"<<endl;
	}*/
//构造分析表
   LAactionTable=actionTable;
   ChangeState(core);
   int width=tActionTable.size()+ntActionTable.size()+1;//此处加1因为留下一个空间存储合并后的状态号
   vector<int> v(width);         //动作表一行的内容
   for(int n=0;n<width;n++)  { v[n]=ERROR;}   //以ERROR值对vector进行初始化
   for(map< int,set<int> >::iterator cor=core.begin();cor!=core.end();cor++){
	   set<int> same=cor->second;
	   for(set<int>::iterator se=same.begin();se!=same.end();se++){
		   for(int num=0;num<width-1;num++)
			   if(LAactionTable[cor->first][num]==LAactionTable[*se][num]){
				   LAactionTable[*se][num]=ERROR;
				   continue;
			   }
			   else if(LAactionTable[cor->first][num]==ERROR){
				   LAactionTable[cor->first][num]=LAactionTable[*se][num];
				   LAactionTable[*se][num]=ERROR;
			   }
			   else if(LAactionTable[*se][num]==ERROR){
				   continue;
			   }
			   else if(LAactionTable[cor->first][num]<=0&&LAactionTable[*se][num]<=0){
				   lout<<"出现归约-归约冲突，文法不是LALR(1)文法！！！"<<endl;
			   }
	   }
   }
   int row=LAactionTable.size();//构造最终分析表
   for(int num1=0;num1<row;num1++)
   {
	   if(core.count(num1)){
		   v[0]=num1;
		   for(int num2=0;num2<LAactionTable[num1].size();num2++)
			   v[num2+1]=LAactionTable[num1][num2];
		   LAactionTableLast.push_back(v);
	   }
	   else{

	   }
   }
   lout<<"临时分析表，用以生成代码："<<endl;
   PrintLAActionTable();
   lout<<"最终分析表："<<endl;
   PrintLAActionTableLast();
}
multimap<set<Item>,int> clearsym(map<set<Item>,int> &itemsetTable)//消去搜索符
{
	set<Item> temp;
    multimap<set<Item>,int> itemsetTable1;//消去搜索符的项目集族
	for(map<set<Item>,int>::iterator i=itemsetTable.begin();i!=itemsetTable.end();i++){
		temp=i->first;
		for(set<Item>::iterator iter=temp.begin();iter!=temp.end();iter++)
		{
			iter->searchSym.clear();
		}	
		pair<set<Item>,int> pi;
		pi.first=temp;
		pi.second=i->second;
		itemsetTable1.insert(pi);
	}
	return itemsetTable1;
}
void merge(int LA,int LR,map<set<Item>,int> &LALRsetTable,map<set<Item>,int> &itemsetTable)//合并同心状态的搜索符
{
	set<Item> first;
	set<Item> second;
	pair<set<Item>,int> pi;
	for(map<set<Item>,int>::iterator i=LALRsetTable.begin();i!=LALRsetTable.end();i++)
	{
		if(i->second==LA){
			first=i->first;
		//	set<Item> SS=i->first;
		//	displayItemSet(SS,i->second);//输出认为是同心的状态的详细信息，认为判断程序是否正确
            LALRsetTable.erase(i);
			break;
		}
	}
	for(map<set<Item>,int>::iterator j=itemsetTable.begin();j!=itemsetTable.end();j++)
	{
		if(j->second==LR){
			second=j->first;
		//	set<Item> SS=j->first;
		//	displayItemSet(SS,j->second);
			break;
		}
	}
	for(set<Item>::iterator i1=first.begin();i1!=first.end();i1++)//合并搜索符
		for(set<Item>::iterator i2=second.begin();i2!=second.end();i2++)
			if(i1->pn==i2->pn)
			{
				for(SET_ITER it=i2->searchSym.begin();it!=i2->searchSym.end();it++)
					i1->searchSym.insert(*it);
			}
	pi.first=first;
	pi.second=LA;
	LALRsetTable.insert(pi);
}
void ChangeState(map< int, set<int> > core){//替换合并后的状态
	int row,column;
	set<int> temp;
	row=LAactionTable.size();
	map< int, set<int> > ::iterator iter;
	column=tActionTable.size()+ntActionTable.size();
	for(int i=0;i<row;i++)
	{
		for(int j=0;j<column;j++)
		{
			if(LAactionTable[i][j]>0)
			{
				for(iter=core.begin();iter!=core.end();iter++)
					if(LAactionTable[i][j]!=iter->first)
					{
						temp=iter->second;
						for(SET_ITER iter1=temp.begin();iter1!=temp.end();iter1++)
							if(LAactionTable[i][j]==*iter1){
								LAactionTable[i][j]=iter->first;
								break;
							}
							else
								continue;
					}
					else
					{
						break;
					}
			}		
		}
	}
}

//生成程序部分
void Print()
{	
	lout<<"\n开始生成程序..."<<endl;
	PrintHeader();              //打印一些常量和函数
	PrintSymTable();
	PrintGetToken();           //打印读token函数
	PrintSemantic();           //打印语义程序
	PrintParser();              //打印总控程序
	PrintInit();
	PrintMain();                //打印生成程序的主函数
	lout<<"生成程序完成"<<endl;
	fout.close();
	lout.close();
	cout<<"Succeed!"<<endl<<"Please enter a letter to exit:";
	char ch;
	cin>>ch;
}
void PrintHeader()
{
	fout<<"#pragma warning(disable :4786)\n";
	fout<<"#include<iostream>\n";
	fout<<"#include<fstream>\n";
    fout<<"#include<map>\n";
    fout<<"#include<stack>\n";  
    fout<<"#include<string>\n";
	fout<<"#include\"yylex.h\"\n";
	fout<<"#include\"yytab.h\"\n";
	fout<<"#define MAXSIZE 256\n";
	fout<<"#define E    32767//错误\n";
	fout<<"#define END  32766//表示文件结束\n";
	fout<<"using namespace std;\n";
	fout<<"struct entry{\n";
	fout<<"\tstring name;\n";
	fout<<"\tint token;\n";
	fout<<"\tint val;\n";
	fout<<"};\n";
    fout<<"struct entry symTable[MAXSIZE];  //符号表\n";
    fout<<"int sympos=0; //符号表初始位置\n";
	fout<<"int val=0; //用来存语义值\n";
    fout<<"int v[256];\n"; 
	fout<<"string name;\n";
	fout<<"ifstream fin;//输入文件\n";
	fout<<"stack<string> id_name;\n";
	fout<<"map<int,int> actionIndex;//符号到动作表位置的映射\n";
	//每个产生式的右部长度，规约时要用到的
	fout<<"int producerN["<<producerSet.size()<<"]={"; 
	int i;
	for(i=0;i<producerSet.size();i++)
	{
		fout<<producerSet[i].right.size();
		if(i!=producerSet.size()-1)
			fout<<",";
		if(i%30==0&&i!=0)
			fout<<"\n              ";
	}
	fout<<"};//产生式的长度\n";
	//产生式的左部编号
	fout<<"int pLeft["<<producerSet.size()<<"]={";    
	for(i=0;i<producerSet.size();i++)
	{
		fout<<producerSet[i].left;
		if(i!=producerSet.size()-1)
			fout<<",";
		if(i%15==0&&i!=0)
			fout<<"\n              ";
	}
	fout<<"};//产生式左部符号\n";
	//动作表初始化
	fout<<"int actionTable["<<LAactionTable.size()<<"]["<<LAactionTable.front().size()<<"]=\n\t\t{";
	for(i=0;i<LAactionTable.size();i++)
	{
		fout<<"\t\t{";
		for(int j=0;j<LAactionTable.front().size();j++)
		{
			if(LAactionTable[i][j]!=ERROR)
				fout<<LAactionTable[i][j];
			else
				fout<<"E";
			if(j==LAactionTable.front().size()-1) continue;
			fout<<",";
		}
		fout<<"}";
		if(i==LAactionTable.size()-1) continue;
		fout<<",\n";
	}
	fout<<"};\n";
	//打印符号与动作表对应的位置表
	fout<<"int searchTable(int row,int column)//查动作表函数\n";
	fout<<"{\n";
	fout<<"\treturn actionTable[row][column];\n";
	fout<<"}\n";
	fout<<"\n";
}
void PrintSymTable()
{
	//打印查符号表函数
	fout<<"int lookup(string x)//查符号表函数\n";
	fout<<"{\n";
	fout<<"\tfor(int i=0;i<=sympos;i++)\n";
	fout<<"\t{\n";
	fout<<"\t\tif(symTable[i].name==x)\n";
	fout<<"\t\t\treturn i;\n";
	fout<<"\t}\n";
	fout<<"\treturn -1;\n";
	fout<<"}\n";
    //打印插入符号表函数
    fout<<"void insert(string x,int y,int z)//插入符号表\n";
	fout<<"{\n";
	fout<<"\tif(lookup(x)==-1)\n";
	fout<<"\t{\n";
	fout<<"\t\tsymTable[sympos].name=x;\n";
	fout<<"\t\tsymTable[sympos].token=y;\n";
	fout<<"\t\tsymTable[sympos].val=z;\n";
	fout<<"\t\tsympos++;\n";
	fout<<"\t}\n";
	fout<<"}\n";
	//打印符号表输出函数
	fout<<"void printsymTable()\n";
	fout<<"{\n";
	fout<<"\tcout<<\"name  ID   val\"<<endl;\n";
	fout<<"\tfor(int i=0;i<sympos;i++)\n";
	fout<<"\t{\n";
	fout<<"\t\tcout<<symTable[i].name<<\" \";\n";
	fout<<"\t\tcout<<symTable[i].token<<\" \";\n";
	fout<<"\t\tcout<<symTable[i].val<<\" \";\n";
	fout<<"\t\tcout<<endl;\n";
	fout<<"\t}\n";
	fout<<"}\n";
	//打印符号表初始化函数
	fout<<"void initsymTable()\n";
	fout<<"{\n";
	fout<<"\tpair<string,int> pi;\n";
	for(map<string,int>::iterator i=terminalTable.begin();i!=terminalTable.end();i++)
	{
		fout<<"\tpi.first=\""<<i->first<<"\";\n";
		fout<<"\tpi.second="<<i->second<<";\n";
		fout<<"\tinsert(pi.first,pi.second,0);\n";
	}
	for(map<string,int>::iterator j=nterminalTable.begin();j!=nterminalTable.end();j++)
	{
		fout<<"\tpi.first=\""<<j->first<<"\";\n";
		fout<<"\tpi.second="<<j->second<<";\n";
		fout<<"\tinsert(pi.first,pi.second,0);\n";
	}
	fout<<"}\n";
	fout.close();
}
void PrintGetToken() 
{	
	fout.open("parse.cpp");
	fout<<"#include \"parse.h\"\n";
	fout<<"int getToken()//得到一个标记\n";
	fout<<"{\n";
	fout<<"\tstring one=\"{}()[]+-*/#;,:\";\n";
	fout<<"\tstring two=\"<>=!\";\n";
	fout<<"\tstring digit=\"0123456789\";\n"; 
	fout<<"\tstring letter=\"0123456789aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ\";\n";
	fout<<"\tchar token[256];\n";
	fout<<"\tint pos=0;\n";
	fout<<"\tchar c=fin.get();\n";
	fout<<"\twhile((c==' '||c=='\\n'||c=='\\r'||c=='\\t')&&!fin.eof()) c=fin.get();\n";
	fout<<"\tif(fin.eof())\n";
	fout<<"\t\treturn END;\n";

	fout<<"\tif(one.find(c)!=-1)//单个符号\n";
	fout<<"\t{\n";
	fout<<"\t\ttoken[pos++]=c;\n";
	fout<<"\t\ttoken[pos]='\\0';\n";
	fout<<"\t}\n";

	fout<<"\telse if(two.find(c)!=-1)//两个符号\n";
	fout<<"\t{\n";
	fout<<"\t\tdo\n";
	fout<<"\t\t{\n";
	fout<<"\t\t\ttoken[pos++]=c;\n";
	fout<<"\t\t\tc=fin.get();\n";
	fout<<"\t\t}while(two.find(c)!=-1);\n";
	fout<<"\t\tfin.putback(c);\n";
	fout<<"\t}\n";

	fout<<"\telse if(digit.find(c)!=-1) //数字\n"; 
	fout<<"\t{\n";
	fout<<"\t\tval=0;\n";
	fout<<"\t\tdo\n";
	fout<<"\t\t{\n";
	fout<<"\t\t\tval=(val*10+c-'0');  //存语义值\n";
	fout<<"\t\t\ttoken[pos++]=c;\n";
	fout<<"\t\t\tc=fin.get();\n";
	fout<<"\t\t}while(digit.find(c)!=-1);\n";
	fout<<"\t\tfin.putback(c);\n";
	fout<<"\t}\n";

	fout<<"\telse if(letter.find(c)!=-1)//标识符或关键字\n";
	fout<<"\t{\n";
	fout<<"\t\tdo\n";
	fout<<"\t\t{\n";
	fout<<"\t\t\ttoken[pos++]=c;\n";
	fout<<"\t\t\tc=fin.get();\n";
	fout<<"\t\t}while(letter.find(c)!=-1);\n";
	fout<<"\t\tfin.putback(c);\n";
	fout<<"\t\ttoken[pos]='\\0';\n";
	fout<<"\t\tname=token;\n";
	fout<<"\t\tid_name.push(name);\n";
	fout<<"\t\treturn analysis(token,strlen(token));\n";
	fout<<"\t}\n";

	fout<<"\telse\n";
	fout<<"\t{\n";
	fout<<"\t\treturn ERROR;\n";
	fout<<"\t}\n";
	fout<<"\ttoken[pos]='\\0';\n";
	fout<<"\treturn analysis(token,strlen(token));\n";
	fout<<"}\n";
}
void PrintInit()
{
	fout<<"void Init()\n";
	fout<<"{\n";
	fout<<"\tpair<int,int> pi;\n";
	for(map<string,int>::iterator i=terminalTable.begin();i!=terminalTable.end();i++)
	{
		fout<<"\tpi.first="<<i->second<<";\n";
		fout<<"\tpi.second="<<tActionTable[i->second]<<";\n";
		fout<<"\tactionIndex.insert(pi);\n";
	}
	for(map<string,int>::iterator j=nterminalTable.begin();j!=nterminalTable.end();j++)
	{
		fout<<"\tpi.first="<<j->second<<";\n";
		fout<<"\tpi.second="<<ntActionTable[j->second]<<";\n";
		fout<<"\tactionIndex.insert(pi);\n";
	}
	fout<<"}\n";
}
void PrintMain()
{
	fout<<"void main()\n";
	fout<<"{\n";
	fout<<"\tInit();\n";
	fout<<"\tstring filename;\n";
	fout<<"\tcout<<\"Please enter the name of input file:\"<<endl;\n";
	fout<<"\tcin>>filename;\n";
	fout<<"\tfin.open(filename.c_str());\n";
	fout<<"\tif(fin.fail())\n";
	fout<<"\t{\n";
	fout<<"\t\tcout<<\"Cannot open the file \"<<filename<<endl;\n";
	fout<<"\t\treturn;\n";
	fout<<"\t}\n";
	fout<<"\tinitsymTable();\n";
	fout<<"\tparse();\n";
	fout<<"\tprintsymTable();\n";
	fout<<"}\n";
}
void PrintSemantic()
{
	fout<<"void semanticAction(int x)\n";
	fout<<"{\n";
	fout<<"\tswitch(x)\n";
	fout<<"\t{\n";
	for(map<int,string>::iterator i=semanticTable.begin();i!=semanticTable.end();i++)
	{
		fout<<"\tcase "<<i->first<<":\n";
		fout<<"\t\t{\n";
		fout<<"\t\t\t"<<i->second<<";\n";
		fout<<"\t\t\tbreak;\n";
		fout<<"\t\t}\n";
	}
	fout<<"\tdefault :\n";
	fout<<"\t\t{\n";
	fout<<"\t\t\tv[0]=0;\n";
	fout<<"\t\t}\n";
	fout<<"\t}\n";
	fout<<"}\n";
}
void PrintParser()
{
	fout<<"int parse()\n";
	fout<<"{\n";
	fout<<"\tstack<int> staStack;//状态栈\n";
	fout<<"\tstack<int> semStack;//语义栈\n"; 
	fout<<"\tstaStack.push(0);\n";
	fout<<"\tsemStack.push('#');\n";  ;
	fout<<"\tint token=getToken();\n";
	fout<<"\twhile(1)\n";
	fout<<"\t{\n";
	fout<<"\t\tint state=staStack.top();\n";

	fout<<"\t\tint action=searchTable(state,actionIndex[token]);\n";
	fout<<"\t\tif(action==E)//出错\n";
	fout<<"\t\t{\n";
	fout<<"\t\t\tcout<<\"Error\"<<endl;\n";
	fout<<"\t\t\treturn 1;\n";
	fout<<"\t\t}\n";
	fout<<"\t\telse if(action<0) //规约动作\n";
	fout<<"\t\t{\n";
	fout<<"\t\t\taction=-action;\n";
	fout<<"\t\t\tint n=producerN[action];\n";
	fout<<"\t\t\tfor(int i=0;i<n;i++)\n";
	fout<<"\t\t\t{\n";
	fout<<"\t\t\t\tstaStack.pop();\n";
	fout<<"\t\t\t\tv[n-i]=semStack.top();\n";
    fout<<"\t\t\t\tsemStack.pop();\n";
	fout<<"\t\t\t}\n";
    fout<<"\t\t\tsemanticAction(action);\n";
	fout<<"\t\t\tsemStack.push(v[0]);\n";
	fout<<"\t\t\tint left=pLeft[action];\n";
	fout<<"\t\t\tstate=staStack.top();\n";
	fout<<"\t\t\tstaStack.push(searchTable(state,actionIndex[left]));\n";
	fout<<"\t\t\tcout<<\"Reduce \"<<action<<endl;\n";
	fout<<"\t\t\tcontinue;\n";
	fout<<"\t\t}\n";
	fout<<"\t\telse if(action==0) //接受状态\n";
	fout<<"\t\t{\n";
	fout<<"\t\t\tcout<<\"Succeed\"<<endl;\n";
	fout<<"\t\t\treturn 0;\n";
	fout<<"\t\t}\n";
	fout<<"\t\telse //移进动作\n";
	fout<<"\t\t{\n";
	fout<<"\t\t\tstaStack.push(action);\n";
	fout<<"\t\t\tsemStack.push(val);\n";
	fout<<"\t\t\tcout<<\"Shift \"<<token<<endl;\n";
	fout<<"\t\t\ttoken=getToken();\n";
	fout<<"\t\t\tif(token==END)\n";
	fout<<"\t\t\t\ttoken=TERMINAL;\n";
	fout<<"\t\t}\n";
	fout<<"\t}\n";
	fout<<"}\n";
}			
//调试时要用到的一些函数		
void PrintTable()
{
	lout<<"--------------产生式----------------------"<<endl;
	int k=0;
	for(vector<Producer>::iterator iter=producerSet.begin();iter!=producerSet.end();iter++)
	{
		lout<<"producer "<<k++<<" "<<iter->left<<":";
		for(vector<int>::iterator v_iter=iter->right.begin();v_iter!=iter->right.end();v_iter++)
		{
			lout<<*v_iter<<" ";
		}
		lout<<endl;
	}
	lout<<"--------------终态符---------------------"<<endl;
	for(M_ITER m=terminalTable.begin();m!=terminalTable.end();m++)
	{
		lout<<m->first<<"\t\t"<<m->second<<endl;
	}
	lout<<"--------------非终态符--------------------"<<endl;
	for(M_ITER n=nterminalTable.begin();n!=nterminalTable.end();n++)
	{
		lout<<n->first<<"\t\t"<<n->second<<endl;
	}
	lout<<"--------------算符优先表--------------------"<<endl;
	for(map<int,int>::iterator l=preTable.begin();l!=preTable.end();l++)
	{
		lout<<l->first<<"\t\t"<<l->second<<endl;
	}
	lout<<"--------------产生式优先表--------------------"<<endl;
	for(map<int,int>::iterator l1=preP.begin();l1!=preP.end();l1++)
	{
		lout<<l1->first<<"\t\t"<<l1->second<<endl;
	}
}
void PrintFirstSet()
{
	lout<<"--------------非终态的首符集--------------"<<endl;
	for(I_ITER i=firstSet.begin();i!=firstSet.end();i++)
	{
		lout<<i->first<<":";
		for(SET_ITER j=i->second.begin();j!=i->second.end();j++)
		{
			lout<<*j<<" ";
		}
		lout<<endl;
	}
}
void PrintActionTable()
{
	map<string,int>::iterator iter;
	int row,column;
	row=actionTable.size();
	column=tActionTable.size()+ntActionTable.size();
	for(int i=0;i<row;i++)
	{
		lout<<i<<"\t";
		for(int j=0;j<column;j++)
		{
			if(actionTable[i][j]==ERROR)
			{ 
				lout<<"e"<<"\t";
			}
			else if(actionTable[i][j]<0)
			{
				lout<<"r"<<-actionTable[i][j]<<"\t";
			}
			else if(actionTable[i][j]==0)
			{
				lout<<"acc"<<"\t";
			}
			else
			{
				lout<<"s"<<actionTable[i][j]<<"\t";
			}		
		}
		lout<<endl;
	}
}
void PrintLAActionTable()
{
	int row,column;
	row=LAactionTable.size();
	column=tActionTable.size()+ntActionTable.size();
	for(int i=0;i<row;i++)
	{
		lout<<i<<"\t";
		for(int j=0;j<column;j++)
		{
			if(LAactionTable[i][j]==ERROR)
			{ 
				lout<<"e"<<"\t";
			}
			else if(LAactionTable[i][j]<0)
			{
				lout<<"r"<<-LAactionTable[i][j]<<"\t";
			}
			else if(LAactionTable[i][j]==0)
			{
				lout<<"acc"<<"\t";
			}
			else
			{
				lout<<"s"<<LAactionTable[i][j]<<"\t";
			}		
		}
		lout<<endl;
	}
}
void PrintLAActionTableLast()
{
	int row,column;
	row=LAactionTableLast.size();
	column=tActionTable.size()+ntActionTable.size()+1;
	for(int i=0;i<row;i++)
	{
		for(int j=0;j<column;j++)
		{
			if(LAactionTableLast[i][j]==ERROR)
			{ 
				lout<<"e"<<"\t";
			}
			else if(LAactionTableLast[i][j]<0)
			{
				lout<<"r"<<-LAactionTableLast[i][j]<<"\t";
			}
			else if(LAactionTableLast[i][j]==0)
			{
				if(j==0)
					lout<<"s"<<LAactionTableLast[i][j]<<"\t";
				else
					lout<<"acc"<<"\t";
			}
			else
			{
				lout<<"s"<<LAactionTableLast[i][j]<<"\t";
			}		
		}
		lout<<endl;
	}
}
void displayItem(Item &item)
{
	cout<<"Item :pn "<<item.pn<<",pos "<<item.pos<<",searchSym(";
	set<int> s=item.searchSym;
	for(set<int>::iterator i=s.begin();i!=s.end();i++)
	{
		cout<<" "<<*i;
	}
	cout<<")\n";
}

void displayItemSet(set<Item> &itemSet,int s)
{
	cout<<"I"<<s<<endl;
	for(set<Item>::iterator i=itemSet.begin();i!=itemSet.end();i++)
		displayItem(*i);
	cout<<endl;
}
