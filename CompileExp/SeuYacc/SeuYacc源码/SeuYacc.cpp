#include "Yacc.h"
int main()
{
	Init();                     //��ʼ������������������ļ� 
	ReadDeclaration();          //��������
	ReadRule();                 //�������
	CreateLR();                //������Ŀ��ͬʱ������Ӧ�Ķ�����
	Print();                   //�������
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
	lout<<"��ʼ��ʼ��..."<<endl;  //��ӡ��־��Ϣ
	//ѹ����չ���ʽ
	Producer ap;
	ap.left=NONTERMINAL-1; //1999
    ap.right.push_back(NONTERMINAL);
	producerSet.push_back(ap);
    //�����ս��'#',���ڵ�һλ��
   	pair<string,int> pi;
	pi.first="TERMINAL";
	pi.second=TERMINAL;    //1000
	terminalTable.insert(pi);
	lout<<"����ʽ��ѹ������չ���ʽ"<<endl;
	lout<<"�ս������������TERMINAL"<<endl;
	lout<<"��ʼ������"<<endl;
}
void ReadDeclaration()
{
	lout<<"\n��ʼ���������Σ��洢�ս��..."<<endl;
	string str;
	bool flag=true;
	getline(fin,str);
	getline(fin,str);  //%{...%}û�����ݣ����Ը�����Ҫ���
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
				terminalTable.insert(pi);   //�����ս������
				c=fin.get();
				while(c==' '||c=='\r'||c=='\t') c=fin.get();
			}
		}
		else if(str=="%left") //����
		{
			priority++; //���ȼ���������ʾ��һ�ж������������ȼ���
			char c=fin.get();
			while(c!='\n')
			{		
				fin.putback(c);
				fin>>str;
				
				pair<string,int> pi;			
				pi.first=str;
				pi.second=TERMINAL+terminalTable.size();
				terminalTable.insert(pi);        //�����ս����
				leftTable.insert(pi.second);  //���������������
				
				pair<int,int> pi1;
				pi1.first=pi.second;
				pi1.second=priority;
				preTable.insert(pi1);  //����������ȱ���
				c=fin.get();
				while(c==' '||c=='\r'||c=='\t') c=fin.get();
			}
		}
		else if(str=="%right")  //�ҽ��
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
				terminalTable.insert(pi);        //�����ս����
				rightTable.insert(pi.second);  //���������������
				
				pair<int,int> pi1;
				pi1.first=pi.second;
				pi1.second=priority;
				preTable.insert(pi1);  //����������ȱ���
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
	lout<<"�����ζ������"<<endl;
}
void ReadRule()
{
	lout<<"\n��ʼ�������ʽ..."<<endl;
	char c=fin.get();
	while(c!='%')
	{
		fin.putback(c);
		ReadProducer();  //������ʽ
		c=fin.get();
		while(c==' '||c=='\n'||c=='\r'||c=='\t') c=fin.get();
	}
	fin.close();  //�����ļ�����

	lout<<"����ʽ�������"<<endl;
	lout<<"\n��ӡ���ж������Ϣ..."<<endl;
	PrintTable();   //��ӡһЩ��Ҫ��Ϣ������ʱ�õ���	
}
void ReadProducer() //�������ʽ�������Ƕ��
{
	Producer ap;//����һ������ʽ
	string str;
	fin>>str;  //�����ķ����
	//��������ս���򱨴�
	if(terminalTable.count(str))  
	{
		cout<<"error:Producer begins with teminal"<<endl;
		exit(1);
	}
	//����Ƿ��ս�������Ҳ��ڷ��ս�������У���ӽ�ȥ
	if(!nterminalTable.count(str)) 
	{
		pair<string,int> pi;
		pi.first=str;
		pi.second=NONTERMINAL+nterminalTable.size();
		nterminalTable.insert(pi);
		ap.left=pi.second;
	}
	//���ս���������ڷ��ս��������
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
		case '\'': //����������,��ʾ�����ս����
			str=fin.get();		
			if(!terminalTable.count(str)) //�ս�������ڱ���,�ӽ���
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
				if(preTable.count(temp))  //�ս�������ȼ�
				{
					pair<int,int> pi;
					pi.first=producerSet.size();
					pi.second=preTable[temp];
					preP.insert(pi);
				}			
			}		
			fin.get(); //��ȡ��һ��'\''
			break;
		case '|':  //һ������������,��ʼ����һ��,�������ǵ��󲿶���һ��,����ֻҪ���Ҳ����������
			producerSet.push_back(ap); 
			ap.right.clear();
			break;
		case ';': //���ﲻҪ���Ǳ������ʽ
			producerSet.push_back(ap); 
			ap.right.clear();
			return;
		case '\n':
		case '\t':
		case '\r':
		case ' ':
			break;
		case '%':  //ǿ�Ƹ�����ʽ�����ȼ�
			{
			while(c!=' ') c=fin.get();
			fin>>str;
			pair<int,int> pi;
			pi.first=producerSet.size();
			pi.second=preTable[terminalTable[str]];
			preP.insert(pi);
			break;
			}
		case '{':  //����������
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
		default:  //�������
			fin.putback(c);
			fin>>str;
			if(terminalTable.count(str))  //�Ѿ�������ս��
			{
				int temp=terminalTable[str];
				ap.right.push_back(temp);
				if(preTable.count(temp))  //�ս�������ȼ�
				{
					pair<int,int> pi;
					pi.first=producerSet.size();
					pi.second=preTable[temp];
					preP.insert(pi);
				}
			}
			else if(!nterminalTable.count(str)) //���ս�������ڱ��У��ӽ���
			{
				pair<string,int> pi;
				pi.first=str;
				pi.second=NONTERMINAL+nterminalTable.size();
				nterminalTable.insert(pi);

				ap.right.push_back(pi.second);
			}			
			else                //���ս�����ڱ���
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
		if(!rhSet.count(ap.left))  //����ʽ�󲿲���rhSet���У��ӽ���
		{
			pair<int,set<int> > pi;
			pi.first=ap.left;
			set<int> s;
			pi.second=s;
			rhSet.insert(pi);
			//������ʼ��lpSet
			lpSet.insert(pi);
		}
		rhSet[ap.left].insert(ap.right[0]);//�������ʽ�Ҳ���һ���ķ�����
		lpSet[ap.left].insert(i);          //������ʼ��lpSet,�����ݽṹ����հ�ʱ�õ�
	}
}
void getFirstSet()  //�����з��ս����first����
{	
	for(I_ITER iter=rhSet.begin();iter!=rhSet.end();iter++)
	{
		if(!firstSet.count(iter->first))//�����û�������������������
		{
			FIRST(iter->first,iter->second);
		}
	}
}
set<int> FIRST(int left,set<int> right)
{	
	set<int>  tSet;
	//��ӵ�firstSet��
	pair<int,set<int> >pi;
	pi.first=left;
	pi.second=tSet;
	firstSet.insert(pi);
    //����һ��ջ��Ҫ����right����������Ԫ��
	stack<int> tStack;
	for(SET_ITER iter=right.begin();iter!=right.end();iter++)	
	{
		tStack.push(*iter);
	}
	while(!tStack.empty())
	{
		int first=tStack.top();
		tStack.pop();
		if(tActionTable.count(first)) //���ս����ֱ����ӵ��׷�����
		{	
			firstSet[left].insert(first);
		}
		else if(firstSet.count(first)) //���ս����firstSet�Ѽ��������
		{
			I_ITER iter=firstSet.find(first);
			for(SET_ITER s_iter=iter->second.begin();s_iter!=iter->second.end();s_iter++)
			{
				firstSet[left].insert(*s_iter);
			}
		}
		else        //���ս����firstSet��û�б����������
		{
			set<int> tSet1=FIRST(first,rhSet[first]);//ѭ�����㣬ֱ�����
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
		tActionTable.insert(pi);//�����ս��������
	}
	for(M_ITER k=nterminalTable.begin();k!=nterminalTable.end();k++)
	{
		pair<int,int> pi;
		pi.first=k->second;
		pi.second=i++;
		ntActionTable.insert(pi);//���ս��������
	}
}
void CreateLR()
{
	lout<<"\n��ʼ����LR(1)�ķ�...\n";
	//׼������
	CalIndex();                 //������̬�������̬���ڷ������е�λ��        
	rightHeadSet();             //�����ͬ�󲿵��Ҳ���
	getFirstSet();              //��÷���̬���׷���
 
	int width=tActionTable.size()+ntActionTable.size();	 //������Ŀ��
    map< set<Item>,int> itemsetTable;  //��Ŀ����
	set<int> S;
	S.insert(TERMINAL);                //TERMINAL��ʾ'#'
	Item firstItem(0,0,S);            //״̬0�ĵ�һ����Ŀ 	
	set<Item> I0;                     //״̬0����Ŀ��
	I0.insert(firstItem);
	CLOSURE(I0);                       //����Ŀ�ıհ�
	queue<set<Item> > Q_item;
	Q_item.push(I0);  
	
	pair<set<Item>,int> pi;          //��I0������Ŀ������
	pi.first=I0;
	pi.second=state;
	itemsetTable.insert(pi);
	//���濪ʼ����Ŀ����
	while(!Q_item.empty())                  //ֱ�����е���Ŀ�����������Ϊֹ
	{        
		vector<int> v(width);         //������һ�е�����
		for(int n=0;n<width;n++)  { v[n]=ERROR;}   //��ERRORֵ��vector���г�ʼ��
		actionTable.push_back(v);   
		
		map<int, set<Item> > moveItemSet;   
		set<Item> ItemSet=Q_item.front();  
		Q_item.pop();
		int column,row;                //�������λ��
		row=itemsetTable[ItemSet];     //��������֪   
		for(set<Item>::iterator i=ItemSet.begin();i!=ItemSet.end();i++)
		{
			if(i->isEnd())          //�ǹ�Լ��Ŀ
			{
				//��������Լ��Ŀ�Ĳ���ʽ��
				for(set<int>::iterator j=i->searchSym.begin();j!=i->searchSym.end();j++)
				{
					//��һ�£��ƽ����Լ���ƽ��ĳ�ͻ
					column=tActionTable[*j];         //��������꣬���ս����teminSet���е�λ��
					if(actionTable[row][column]!=ERROR)
					{
						//�޷�������������������˳���
						lout<<"��"<<row<<":���ڹ�Լ����Լ��ͻ���޷������������ֹ��"<<endl;  //��ӡ��־
						exit(1);
					}
					actionTable[row][column]=-(i->pn);   //�����,��Լ��Ŀ�ø��ű�ʾ,0�ǽ���̬
				}
			}
			else                       //���ǹ�Լ��Ŀ���ƽ�һ������
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
		//����move���������Ŀ��
		for(map<int,set<Item> >::iterator k=moveItemSet.begin();k!=moveItemSet.end();k++)
		{
			CLOSURE(k->second);	
			if(!itemsetTable.count(k->second))     //������Ŀ����Ҫ�ӽ�����
			{
				pair<set<Item>,int> pi;       
				pi.first=k->second;			
				pi.second=++state;
				itemsetTable.insert(pi);
		
				Q_item.push(k->second);    //�ӵ�������		
			}	
			//��ʼ���ƽ���
			column=(ntActionTable.count(k->first)?ntActionTable[k->first]:tActionTable[k->first]);
			if(actionTable[row][column]!=ERROR)  //����ֻ������ƽ���Լ��ͻ
			{
				lout<<"��"<<row<<":�����ƽ�����Լ��ͻ"<<endl;
				if(preTable[k->first]>preP[-actionTable[row][column]])
				{
					actionTable[row][column]=itemsetTable[k->second];
					lout<<"�ƽ������ȼ��ߣ��ƽ�"<<endl;
					continue;
				}
				else
				{
					lout<<"�ƽ������ȼ��ͣ�������"<<endl;
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
	lout<<"LR(1)�ķ��������\n"<<"\n��ʼ��ӡ������\n";
	PrintFirstSet();
	PrintActionTable();         //��ӡ������
	lout<<"\nLR(1)ӳ�䵽LALR(1)\n";
	LRtoLALR(itemsetTable);

}

void CLOSURE(set<Item> &I) //����Ŀ���ıհ�����
{  
	queue<Item> Q_item;
	
	for(set<Item>::iterator i=I.begin();i!=I.end();i++){ Q_item.push(*i); }  //��ʼ������I��ѹ�������
    while(!Q_item.empty())  
	{
		Item item=Q_item.front();
		Q_item.pop();		
		I.insert(item);	
		int next=item.getNextSym();//����һ��Ҫ�ƽ��ķ���
        if(next!=-1&&!tActionTable.count(next))  //��Ϊ���Լ������ս���Ŵ������������Ŀ��������
		{
			set<int> np=lpSet[next];
			int nnext=item.getNNextSym();
			for(set<int>::iterator j=np.begin();j!=np.end();j++)
			{
				set<int> Sym;
				if(nnext==-1) { Sym=item.searchSym; }
				else if(tActionTable.count(nnext))  { Sym.insert(nnext); }
				else          { Sym=firstSet[nnext]; }
				Item nitem(*j,0,Sym); //��������Ŀ
				if(!I.count(nitem))
				{
					Q_item.push(nitem);
				}
			}
		}//end if
	}//end while
	//����Ŀ��
	set<Item> result;
	for(set<Item>::iterator n=I.begin();n!=I.end();n++)
	{
		for(set<Item>::iterator m=I.begin();m!=I.end();m++)
		{
			if(!n->IsMerge(*m)) continue; //����Ҫ�ϲ�
			n->Merge(*m);                  //�ϲ�   

		}
		if(!result.count(*n)) 
			result.insert(*n);
	}			
	I=result;
}
//LR->LALR����
void LRtoLALR(map<set<Item>,int> &itemsetTable)
{
	lout<<"��ʼ�ϲ�ͬ��״̬\n";
	map< set<Item>,int> LALRsetTable;//����LALR(1)��Ŀ����
    map< int, set<int> > core;//��¼ͬ�ĵĸ�״̬,�ؼ����ǵ�һ������״̬��ֵ�Ǻ�������֮ͬ�ĵ�״̬
	map< set<Item>,int> temp;//��ʱ��¼��״̬
	multimap<set<Item>,int> itemsetTable1;//��ȥ�����������Ŀ����
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
			if(!core.count(temp[i->first]))//ͬ��״̬δ��¼,�½�pair����
			{
				pair<int, set<int> > pi1;
				pi1.first=temp[i->first];
				pi1.second.insert(i->second);
				core.insert(pi1);
			}
			else//ͬ��״̬�Ѽ�¼��ֱ����ӵ�����
			{
				core[temp[i->first]].insert(i->second);
			}
		}//����ͬ��״̬���
   lout<<"ͬ��״̬�ϲ���ϣ���ʼ����������\n";
  /* cout<<core.size()<<endl;//����ҵ���ͨͬ��״̬����
   for(map< int,set<int> >::iterator i2=core.begin();i2!=core.end();i2++)
	{
		set<int> co=i2->second;
		cout<<"{"<<i2->first;
		for(set<int>::iterator i3=co.begin();i3!=co.end();i3++)
			cout<<","<<*i3;
		cout<<"}"<<endl;
	}*/
//���������
   LAactionTable=actionTable;
   ChangeState(core);
   int width=tActionTable.size()+ntActionTable.size()+1;//�˴���1��Ϊ����һ���ռ�洢�ϲ����״̬��
   vector<int> v(width);         //������һ�е�����
   for(int n=0;n<width;n++)  { v[n]=ERROR;}   //��ERRORֵ��vector���г�ʼ��
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
				   lout<<"���ֹ�Լ-��Լ��ͻ���ķ�����LALR(1)�ķ�������"<<endl;
			   }
	   }
   }
   int row=LAactionTable.size();//�������շ�����
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
   lout<<"��ʱ�������������ɴ��룺"<<endl;
   PrintLAActionTable();
   lout<<"���շ�����"<<endl;
   PrintLAActionTableLast();
}
multimap<set<Item>,int> clearsym(map<set<Item>,int> &itemsetTable)//��ȥ������
{
	set<Item> temp;
    multimap<set<Item>,int> itemsetTable1;//��ȥ����������Ŀ����
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
void merge(int LA,int LR,map<set<Item>,int> &LALRsetTable,map<set<Item>,int> &itemsetTable)//�ϲ�ͬ��״̬��������
{
	set<Item> first;
	set<Item> second;
	pair<set<Item>,int> pi;
	for(map<set<Item>,int>::iterator i=LALRsetTable.begin();i!=LALRsetTable.end();i++)
	{
		if(i->second==LA){
			first=i->first;
		//	set<Item> SS=i->first;
		//	displayItemSet(SS,i->second);//�����Ϊ��ͬ�ĵ�״̬����ϸ��Ϣ����Ϊ�жϳ����Ƿ���ȷ
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
	for(set<Item>::iterator i1=first.begin();i1!=first.end();i1++)//�ϲ�������
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
void ChangeState(map< int, set<int> > core){//�滻�ϲ����״̬
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

//���ɳ��򲿷�
void Print()
{	
	lout<<"\n��ʼ���ɳ���..."<<endl;
	PrintHeader();              //��ӡһЩ�����ͺ���
	PrintSymTable();
	PrintGetToken();           //��ӡ��token����
	PrintSemantic();           //��ӡ�������
	PrintParser();              //��ӡ�ܿس���
	PrintInit();
	PrintMain();                //��ӡ���ɳ����������
	lout<<"���ɳ������"<<endl;
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
	fout<<"#define E    32767//����\n";
	fout<<"#define END  32766//��ʾ�ļ�����\n";
	fout<<"using namespace std;\n";
	fout<<"struct entry{\n";
	fout<<"\tstring name;\n";
	fout<<"\tint token;\n";
	fout<<"\tint val;\n";
	fout<<"};\n";
    fout<<"struct entry symTable[MAXSIZE];  //���ű�\n";
    fout<<"int sympos=0; //���ű��ʼλ��\n";
	fout<<"int val=0; //����������ֵ\n";
    fout<<"int v[256];\n"; 
	fout<<"string name;\n";
	fout<<"ifstream fin;//�����ļ�\n";
	fout<<"stack<string> id_name;\n";
	fout<<"map<int,int> actionIndex;//���ŵ�������λ�õ�ӳ��\n";
	//ÿ������ʽ���Ҳ����ȣ���ԼʱҪ�õ���
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
	fout<<"};//����ʽ�ĳ���\n";
	//����ʽ���󲿱��
	fout<<"int pLeft["<<producerSet.size()<<"]={";    
	for(i=0;i<producerSet.size();i++)
	{
		fout<<producerSet[i].left;
		if(i!=producerSet.size()-1)
			fout<<",";
		if(i%15==0&&i!=0)
			fout<<"\n              ";
	}
	fout<<"};//����ʽ�󲿷���\n";
	//�������ʼ��
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
	//��ӡ�����붯�����Ӧ��λ�ñ�
	fout<<"int searchTable(int row,int column)//�鶯������\n";
	fout<<"{\n";
	fout<<"\treturn actionTable[row][column];\n";
	fout<<"}\n";
	fout<<"\n";
}
void PrintSymTable()
{
	//��ӡ����ű���
	fout<<"int lookup(string x)//����ű���\n";
	fout<<"{\n";
	fout<<"\tfor(int i=0;i<=sympos;i++)\n";
	fout<<"\t{\n";
	fout<<"\t\tif(symTable[i].name==x)\n";
	fout<<"\t\t\treturn i;\n";
	fout<<"\t}\n";
	fout<<"\treturn -1;\n";
	fout<<"}\n";
    //��ӡ������ű���
    fout<<"void insert(string x,int y,int z)//������ű�\n";
	fout<<"{\n";
	fout<<"\tif(lookup(x)==-1)\n";
	fout<<"\t{\n";
	fout<<"\t\tsymTable[sympos].name=x;\n";
	fout<<"\t\tsymTable[sympos].token=y;\n";
	fout<<"\t\tsymTable[sympos].val=z;\n";
	fout<<"\t\tsympos++;\n";
	fout<<"\t}\n";
	fout<<"}\n";
	//��ӡ���ű��������
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
	//��ӡ���ű��ʼ������
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
	fout<<"int getToken()//�õ�һ�����\n";
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

	fout<<"\tif(one.find(c)!=-1)//��������\n";
	fout<<"\t{\n";
	fout<<"\t\ttoken[pos++]=c;\n";
	fout<<"\t\ttoken[pos]='\\0';\n";
	fout<<"\t}\n";

	fout<<"\telse if(two.find(c)!=-1)//��������\n";
	fout<<"\t{\n";
	fout<<"\t\tdo\n";
	fout<<"\t\t{\n";
	fout<<"\t\t\ttoken[pos++]=c;\n";
	fout<<"\t\t\tc=fin.get();\n";
	fout<<"\t\t}while(two.find(c)!=-1);\n";
	fout<<"\t\tfin.putback(c);\n";
	fout<<"\t}\n";

	fout<<"\telse if(digit.find(c)!=-1) //����\n"; 
	fout<<"\t{\n";
	fout<<"\t\tval=0;\n";
	fout<<"\t\tdo\n";
	fout<<"\t\t{\n";
	fout<<"\t\t\tval=(val*10+c-'0');  //������ֵ\n";
	fout<<"\t\t\ttoken[pos++]=c;\n";
	fout<<"\t\t\tc=fin.get();\n";
	fout<<"\t\t}while(digit.find(c)!=-1);\n";
	fout<<"\t\tfin.putback(c);\n";
	fout<<"\t}\n";

	fout<<"\telse if(letter.find(c)!=-1)//��ʶ����ؼ���\n";
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
	fout<<"\tstack<int> staStack;//״̬ջ\n";
	fout<<"\tstack<int> semStack;//����ջ\n"; 
	fout<<"\tstaStack.push(0);\n";
	fout<<"\tsemStack.push('#');\n";  ;
	fout<<"\tint token=getToken();\n";
	fout<<"\twhile(1)\n";
	fout<<"\t{\n";
	fout<<"\t\tint state=staStack.top();\n";

	fout<<"\t\tint action=searchTable(state,actionIndex[token]);\n";
	fout<<"\t\tif(action==E)//����\n";
	fout<<"\t\t{\n";
	fout<<"\t\t\tcout<<\"Error\"<<endl;\n";
	fout<<"\t\t\treturn 1;\n";
	fout<<"\t\t}\n";
	fout<<"\t\telse if(action<0) //��Լ����\n";
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
	fout<<"\t\telse if(action==0) //����״̬\n";
	fout<<"\t\t{\n";
	fout<<"\t\t\tcout<<\"Succeed\"<<endl;\n";
	fout<<"\t\t\treturn 0;\n";
	fout<<"\t\t}\n";
	fout<<"\t\telse //�ƽ�����\n";
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
//����ʱҪ�õ���һЩ����		
void PrintTable()
{
	lout<<"--------------����ʽ----------------------"<<endl;
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
	lout<<"--------------��̬��---------------------"<<endl;
	for(M_ITER m=terminalTable.begin();m!=terminalTable.end();m++)
	{
		lout<<m->first<<"\t\t"<<m->second<<endl;
	}
	lout<<"--------------����̬��--------------------"<<endl;
	for(M_ITER n=nterminalTable.begin();n!=nterminalTable.end();n++)
	{
		lout<<n->first<<"\t\t"<<n->second<<endl;
	}
	lout<<"--------------������ȱ�--------------------"<<endl;
	for(map<int,int>::iterator l=preTable.begin();l!=preTable.end();l++)
	{
		lout<<l->first<<"\t\t"<<l->second<<endl;
	}
	lout<<"--------------����ʽ���ȱ�--------------------"<<endl;
	for(map<int,int>::iterator l1=preP.begin();l1!=preP.end();l1++)
	{
		lout<<l1->first<<"\t\t"<<l1->second<<endl;
	}
}
void PrintFirstSet()
{
	lout<<"--------------����̬���׷���--------------"<<endl;
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
