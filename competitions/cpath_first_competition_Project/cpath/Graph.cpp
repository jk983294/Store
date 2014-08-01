#include <fstream>
#include <sstream>
#include "Graph.h"
#include "FibonacciHeap.h"
#include "BinaryHeap.h"

using namespace std;

void Graph::calculate_all(){
	if (algotype == Naive){
		bfs_shortest_path(true);											//BFS�ҳ���·��
		naive_record_primary_path();									//��¼��·��
		if(constraintype == EdgeDisjoint)	naive_calculate_backup_path_EdgeDisjoint();			//ɾ����·���ߺ����󱸷�·��
		else naive_calculate_backup_path_NodeDisjoint();		//ɾ����·���ڵ�����󱸷�·��
	}
	else if(algotype == Optimal){
		if(constraintype == NodeDisjoint)		split_vertices();	
		//�ô����·������ȫ���ڵ㶼���꣬��Ϊ������Ҫ���·����ֵ���޸�Ȩֵ	
		bfs_shortest_path(false);						//��������Ȩͼ��ʹ��BSF�����·������������ٶ�O(V+E),�ڵ�Լ�������·��Ҳ��ͨ��bfs���,��Ϊ��ֱ�ȨֵΪ0������Ȩֵ=���Ȩֵ+0
//		dijkstra_shortest_path(false);					//O(VlgV+E),�������Ȩ��������·��
		modify_weight_create_residual_graph();
		insert_and_erase_overlap_edge();				//��¼��·��
		//�ô�ֻ��Ҫ��õڶ���·�����ɣ�����Ҫȫ������
		dijkstra_shortest_path(true);		
		insert_and_erase_overlap_edge();				//��¼����
		reorganise_path();
	}	
	else if (algotype == MaxFlow){
		if(constraintype == NodeDisjoint)		split_vertices();	
		Edmonds_Karp();
		reorganise_path();
	}
}
void Graph::reorganise_path(){
	multimap<int,int>::iterator itr;
	int current, path1len=-1,	path2len = -1, tmppathlen;
	for (int i =0; i< pathNum; ++i){
		vector<int> tmppath;
		current = source;
		while(current != dest){
			if ( current < half_n){
				tmppath.push_back(current);
			}
			itr = nonoverlap_edges.find(current);
			current = itr->second;
			nonoverlap_edges.erase(itr);
		}
		if(constraintype == NodeDisjoint)	current -= half_n;
		tmppath.push_back(current);
		tmppathlen = tmppath.size();
		if (path1len < 0 || tmppathlen < path1len){
/*			print_result("other path1: ",backup_path);*/
			backup_path.swap(primary_path);	path2len = path1len;
			primary_path.swap(tmppath);	path1len = tmppathlen;
		}
		else if (path2len < 0 || tmppathlen < path2len )	{
/*			print_result("other path2: ",backup_path);*/
			backup_path.swap(tmppath);	path2len = tmppathlen;
		}
// 		else{
// 			print_result("other path3: ",tmppath);
// 		}
	}
}
void Graph::bfs_edmonds_karp(){
	for (int i = 0; i< n; ++i){parent[i] = -1;     distance[i] = -1;}
	int current = source ;
	parent[current] = current;      distance[current] = 0;
	queue<int> q;
	q.push(current);
	while (! q.empty()){
		current = q.front();    q.pop();
		//add every node w adjcent to v into stack
		list<Node>::iterator itr = linktable[current].begin();
		while(itr !=  linktable[current].end()){
			if ( itr->residual_weight > 0 && distance[itr->id] < 0){	//��������������û�б����ʹ�
				parent[itr->id] = current;
				distance[itr->id] = 0;		//��ʾ�����ʹ�
				if ( itr->id==dest){	return;	}
				q.push(itr->id);
			}
			++itr;
		}
	}
}
void Graph::Edmonds_Karp(){
	//residual_capacity ÿ�μ����궼��仯�������µ�һ�μ�����Ҫ���³�ʼ��
	int  u, v , flow=0, aug;
	list<Node>::iterator itr;
	while(true){
		bfs_edmonds_karp();
		if( parent[dest] < 0 )    break;        //����������·
		aug=0x7fff;    //��¼��С��������
		for( u=parent[v=dest]; v!=source; v=u,u=parent[u] )   //����ǰ���¼�������������·�������С��������
		{
			itr = find_edge(u,v);
			if(itr->residual_weight < aug)    aug=itr->residual_weight;
		}
		for( u=parent[v=dest]; v!=source; v=u,u=parent[u] )   //�������·��������С�������������޸�����
		{
			itr = find_edge(u,v);	itr->residual_weight-=aug;
			itr = find_edge(v,u);
			if(itr == linktable[v].end())	add_edge(v,u,aug,false);
			else	itr->residual_weight+=aug;
		}
		insert_and_erase_overlap_edge();
		flow+= aug;
	}	
/*	cout<<flow<<endl;*/
}
void Graph::insert_and_erase_overlap_edge(){
	pair<int,int> edge;
	bool isfindreverse;
	multimap<int,int>::iterator itr;
	int pre, current, num;
	if(parent[dest] >= 0){		//��Path�еı���map�еı߱Ƚϣ�һ���з���ߵľ�ɾ��	
		++pathNum,	pre = 0, current = dest;
		while(current != source){
			isfindreverse = false;
			pre = parent[current];
			edge.first = pre;	edge.second = current;
			num = nonoverlap_edges.count(current);
			itr = nonoverlap_edges.find(current );
			for (int i=0; i< num; ++i){
				if(itr->second == pre){		//�ҵ������
/*					cout<<"reverse edge: "<<itr->first<<"  "<<itr->second<<endl;*/
					nonoverlap_edges.erase(itr);	isfindreverse = true;
					break;
				}
			}
			if ( ! isfindreverse){	nonoverlap_edges.insert(edge);	}
			current = pre;
		}
	}
}
void Graph::modify_weight_create_residual_graph(){
	list<Node>::iterator itrtmp;
	for (int i = 0; i < n; ++i){		//�޸�Ȩֵ
		list<Node>::iterator itr = linktable[i].begin();
		while(itr !=  linktable[i].end()){
			if (itr->id == source){		//ɾ��ָ��source�ı�
				itrtmp = itr;	++itr;
				linktable[i].erase(itrtmp);
			}
			else{
				itr->weight = itr->weight - distance[itr->id] + distance[i];
				++itr;
			}		
		}
	}
	//��·��P1�е�0Ȩ�ر߷���
	if(parent[dest] >= 0){		
		int pre = 0, current = dest;
		while(current != source){
			pre = parent[current];
			remove_edge(pre,current,false);
			set_edge(current,pre,0);
			current = pre;
		}
	}

}
void Graph::dijkstra_shortest_path(bool isTerminateAsFound){
	//<length nodeid > tricky nodes[i]Ϊ�ڵ�i��heap�е�ָ��
// 	FibonacciHeap<int, int > q;  //���ȶ���
// 	vector <FibonacciHeapNode<int, int >*> nodes(n);  
	MinBinaryHeap<int, int > q;  //���ȶ���	
	vector <BinaryHeapNode<int, int >*> nodes(n);   	
	int min,w;		//min ��СȨֵ��Ӧ�Ľڵ�id, wΪ��min�����Ľڵ�id
	for (int i = 0; i < n; ++i){
		distance[i] = POSTIVE_MAXINT;     parent[i] = -1;
		if ( i == source ) { distance[i] = 0; }
		nodes[i] = q.insert(distance[i],i);
	}
	while(!q.empty()){
		min = q.minimum()->data();
		if (isTerminateAsFound  && min==dest){	//����ȵ�dest�����ȶ��в��ܽ�������Ϊ�Ժ󶼲����ܶ����ɳ�
			break;
		}	
		if (q.minimum()->key() == POSTIVE_MAXINT){	break;	}	//����Ķ�������ɭ�֣�����Ҫ����
//		visitNode(min);
		q.removeMinimum();  nodes[min] = NULL;
		//��ʼ���º�u�����ı� relax
		list<Node>::iterator itr = linktable[min].begin();
		while(itr !=  linktable[min].end()){
			w = itr->id;
			if (nodes[w] != NULL)   //δ��ѡ�м���S
			{
				if (distance[min]+itr->weight < distance[w])    //relax
				{
					q.decreaseKey(nodes[w], distance[min]+itr->weight);
					distance[w] = distance[min]+itr->weight;
					parent[w] = min;
				}
			}
			++itr;
		}
	}
}
void Graph::bfs_shortest_path(bool isTerminateAsFound){
	for (int i = 0; i< n; ++i){
		parent[i] = -1;     distance[i] = -1;
	}
	int current = source ;
	parent[current] = current;      distance[current] = 0;
	bool OuterLoopTerminate = false;
	/*	visitNode(current);*/
	queue<int> q;
	q.push(current);
	while (! q.empty()){
		current = q.front();    q.pop();
		//add every node w adjcent to v into stack
		list<Node>::iterator itr = linktable[current].begin();
		while(itr !=  linktable[current].end()){
			if (distance[itr->id] < 0)
			{
				parent[itr->id] = current;
				distance[itr->id] = distance[current] + itr->weight;
				//����ȵ�dest�����вŽ�������Ϊ�Ժ󶼲����dest�ɳ�
				if (isTerminateAsFound  && itr->id==dest){	OuterLoopTerminate = true;	break;	}
/*				visitNode(itr->id);*/
				q.push(itr->id);
			}
			++itr;
		}
		if (isTerminateAsFound  && OuterLoopTerminate){	break;}
	}
}
void Graph::naive_calculate_backup_path_NodeDisjoint(){
	if(primary_path.size() > 2){    //�м�������һ���ڵ�
		for(unsigned int i =1; i < primary_path.size() -1; ++i){
			remove_node(primary_path[i]);
		}
		bfs_shortest_path(true);
		naive_record_backup_path();
	}
}
void Graph::naive_calculate_backup_path_EdgeDisjoint(){
	if(primary_path.size() > 1){    //����һ����
		for(unsigned int i =1; i < primary_path.size(); ++i){
			remove_edge(primary_path[i-1],primary_path[i],true);
		}
		bfs_shortest_path(true);
		naive_record_backup_path();
	}
}
void Graph::naive_record_primary_path(){
	int current;
	if(distance[dest] > 0){
		current = dest;
		while(current != source){
			primary_path.push_back(current);
			current = parent[current];
		}
		primary_path.push_back(source);
		reverse(primary_path.begin(), primary_path.end());
	}
}
void Graph::naive_record_backup_path(){
	int current;
	if(distance[dest] > 0){
		current = dest;
		while(current != source){
			backup_path.push_back(current);
			current = parent[current];
		}
		backup_path.push_back(source);
		reverse(backup_path.begin(), backup_path.end());
	}
// 	if(distance[dest] > 0){
// 		backup_path.resize(distance[dest]+1);
// 		int index = 0, current = dest;
// 		while(current != source){
// 			backup_path[index++] = current;
// 			current = parent[current];
// 		}
// 		backup_path[index] = source;
// 		reverse(backup_path.begin(), backup_path.end());
// 	}
}


void Graph::remove_edge(int v,int w, bool isbilinkremove){
// 	EqualNodeId eni(w);
// 	linktable[v].remove_if(eni);
	remove_edge(v,w);
	if (isbilinkremove){
// 		eni.id = v;
// 		linktable[w].remove_if(eni);
		remove_edge(w,v);
	}
}
void Graph::remove_node(int v){
	list<Node>::iterator itr = linktable[v].begin();
	while(itr != linktable[v].end()){
		remove_edge(itr->id,v);
		++itr;
	}
	linktable[v].clear();
}
void Graph::add_edge(int v,int w,int weight, bool is_bilink){
	Node tmpnode(w,weight);
	linktable[v].push_front(tmpnode);  //���뵽�ڽӱ�
	if (is_bilink){
		tmpnode.id=v;
		linktable[w].push_front(tmpnode);
	}
}
void Graph::set_edge(int v,int w,int weight){
	list<Node>::iterator itr = linktable[v].begin();
	while(itr != linktable[v].end()){
		if (itr->id == w)	{
			itr->weight = weight;
			break;
		}
		++itr;
	}
	if(itr == linktable[v].end()){	add_edge(v,w,weight,false);	}
}
Graph::Graph(){
	constraintype = EdgeDisjoint;
	pathNum = 0;
	resize(MAX_NODE_NUM);
}
void Graph::resize(int capacity){
	n = capacity;
	linktable.resize(n);
	distance.resize(n);
	parent.resize(n);
}
Graph::~Graph(){}
bool Graph::read_inputfile(string path){
	ifstream ifs(path.c_str());
	if (!ifs.good()){
		cout<<"�Ҳ���ָ�������ļ�"<<endl;
		return false;
	}	
	string line;
	stringstream ss;
	int node1,	node2,	weight = 1, max_node_id =0,	index;
	
	getline(ifs, line);
	while(getline(ifs, line)){
		if(line.size() == 0) continue;
		index = line.find(',',0);
		line[index] = ' ';
		ss.clear();	
		ss<<line;
		ss>>node1>>node2;
		max_node_id = max_node_id<node1 ? node1 : max_node_id;
		max_node_id = max_node_id<node2 ? node2 : max_node_id;
		if (inputHasWeight)
		{
			ss>>weight;	
/*			cout<<node1<<" "<<node2<<" "<<weight<<endl;*/
		}    
		add_edge(node1,node2,weight,true);
	}
	half_n = n = max_node_id +1;
	ifs.close();
	return true;
}
void Graph::print_result(string outputfile){
	ofstream ofs(outputfile.c_str());
	ofs<<"main: ";
	if (primary_path.size() > 0){
		ofs<<primary_path[0];
		for(unsigned int i = 1; i < primary_path.size(); ++i)
			ofs<<", "<<primary_path[i];
	}	
	ofs<<endl<<"backup: ";
	if (backup_path.size() > 0){
		ofs<<backup_path[0];
		for(unsigned int i = 1; i < backup_path.size(); ++i)
			ofs<<", "<<backup_path[i];
	}
}
void Graph::print_result(string description, vector<int> &path){	
	cout<<description<<path.size()<<endl;
	for(unsigned int i = 0; i < path.size(); ++i){
		cout<<path[i]<<" ";
	}cout<<endl;
}
void Graph::split_vertices(){
	half_n = n;	n *= 2;	
	int weight = 0;
	if(algotype == MaxFlow)	weight = 1;
	if (n > MAX_NODE_NUM){		resize(n);	}
	for (int i = 0; i < half_n; ++i){
		add_edge(i+half_n, i , weight, false);		//��ֺ�Ľڵ㵥�������,ȨֵΪweight
	}
	for (int i = 0; i < half_n; ++i){
		list<Node>::iterator itr = linktable[i].begin();
		while(itr !=  linktable[i].end()){
			itr->id += half_n;		//��ڵ��Ϊid+half_n
			++itr;
		}
	}
	dest += half_n;		//Ŀ�Ľڵ��Ϊ������ڵ�
}
void Graph::visitNode(int current){
	cout<<current<<" "<<parent[current]<<" "<<distance[current]<<endl;
}
void Graph::print_info(){
	cout<<"edges :"<<endl;
	for (int i = 0; i < n; ++i){
		list<Node>::iterator itr = linktable[i].begin();
		while(itr !=  linktable[i].end()){
			cout<<"edge ( "<<i<<" , "<<itr->id<<" ) weight : "<<itr->weight<<endl;
			++itr;
		}
	}
}
bool Graph::islinked(int v,int w){
	list<Node>::iterator itr = linktable[v].begin();
	while(itr !=  linktable[v].end()){
		if (itr->id == w)	return true;
		++itr;
	}
	return false;
}
list<Node>::iterator Graph::find_edge(int v,int w){
	list<Node>::iterator itr = linktable[v].begin();
	while(itr !=  linktable[v].end()){
		if (itr->id == w)	return itr;
		++itr;
	}
	return itr;
}
void Graph::remove_edge(int v,int w){
	list<Node>::iterator itr = linktable[v].begin();
	while(itr !=  linktable[v].end()){
		if (itr->id == w){
			linktable[v].erase(itr);	return;
		}
		++itr;
	}
}
void Graph::validate_correctness(string resultpath){
	ifstream ifs(resultpath.c_str());
	if(!ifs.good())	return;
	string line;
	stringstream ss;
	int node1,node2,len;
	if (constraintype == NodeDisjoint){	//����ʼ���Ŀ�����ͬ����������ͬ
		set<int> nodeset;	
		getline(ifs,line);		//������·��
		replace(line.begin(),line.end(),',',' ');
		ss.clear();	ss<<line; ss>>line;	len =0;
		while(ss>>node2){
			len++;
			if (node2 == source){	node1 = node2;	continue;}	
			if (!islinked(node1,node2)){	cout<<"��·����û�б� "<<node1<<" , "<<node2<<endl;	return;}
			else {cout<<"��·�����б� "<<node1<<" , "<<node2<<endl;}
			if(node2 != dest && node2 != source){
				if (nodeset.find(node2) == nodeset.end()){	nodeset.insert(node2);	}
				else{	cout<<"��·���ڵ� "<<node2<<" �ظ�!!!"<<endl;	return;}
			}	
			node1 = node2;
		}
		cout<<"��·����: "<<len-1<<endl;
		getline(ifs,line);		//���뱸��·��
		replace(line.begin(),line.end(),',',' ');
		ss.clear();	ss<<line; ss>>line;	len =0;
		while(ss>>node2){
			len++;
			if (node2 == source){	node1 = node2;	continue;}	
			if (!islinked(node1,node2)){	cout<<"����·����û�б� "<<node1<<" , "<<node2<<endl;	return;}
			else {cout<<"����·�����б� "<<node1<<" , "<<node2<<endl;}
			if(node2 != dest && node2 != source){
				if (nodeset.find(node2) == nodeset.end()){nodeset.insert(node2);}
				else{	cout<<"����·���ڵ� "<<node2<<" �ظ�!!!"<<endl;	return;}
			}	
			node1 = node2;
		}
		cout<<"����·����: "<<len-1<<endl;
	}
	else if((constraintype == EdgeDisjoint)){
		map<int,int> edgeset;
		pair<int,int> edge;
		getline(ifs,line);		//������·��
		replace(line.begin(),line.end(),',',' ');
		ss.clear();	ss<<line; ss>>line;	len =0;
		while(ss>>node2){
			len++;
			if (node2 == source){	node1 = node2;	continue;}	
			if (!islinked(node1,node2)){	cout<<"��·����û�б� "<<node1<<" , "<<node2<<endl;	return;}
			else {
				cout<<"��·�����б� "<<node1<<" , "<<node2<<endl;
				if (node1 > node2){	edge.first = node1;	edge.second = node2;}
				else {	edge.first = node2;	edge.second = node1;}
				if (edgeset.find(edge.first) != edgeset.end() && edgeset[edge.first] == edge.second)
				{	cout<<"��·���ڱ� "<<node1<<" , "<<node2<<" �ظ�!!!"<<endl;	return;}
				else{	edgeset.insert(edge);}
			}
			node1 = node2;
		}
		cout<<"��·����: "<<len-1<<endl;
		getline(ifs,line);		//���뱸��·��
		replace(line.begin(),line.end(),',',' ');
		ss.clear();	ss<<line; ss>>line;	len =0;
		while(ss>>node2){
			len++;
			if (node2 == source){	node1 = node2;	continue;}	
			if (!islinked(node1,node2)){	cout<<"����·����û�б� "<<node1<<" , "<<node2<<endl;	return;}
			else {
				cout<<"����·�����б� "<<node1<<" , "<<node2<<endl;
				if (node1 > node2){	edge.first = node1;	edge.second = node2;}
				else {	edge.first = node2;	edge.second = node1;}
				if (edgeset.find(edge.first) != edgeset.end() && edgeset[edge.first] == edge.second)
				{	cout<<"����·���ڱ� "<<node1<<" , "<<node2<<" �ظ�!!!"<<endl;	return;}
				else{	edgeset.insert(edge);}
			}
			node1 = node2;
		}
		cout<<"����·����: "<<len-1<<endl;
	}
	cout<<"��ȷ"<<endl;
}