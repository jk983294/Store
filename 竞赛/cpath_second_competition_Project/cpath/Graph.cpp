#include <fstream>
#include <sstream>
#include "Graph.h"
#include "BinaryHeap.h"

using namespace std;

void Graph::calculate_path_pass_spefixed_nodes(){
	if (algotype == DynamicProgramming){
		do {
			dynamic_programming();
		} while (check_if_has_duplicate_nodes());
	}else{
		insert_sort(mustpass_nodes,1,middlecnt+1);		//�м�ڵ㰴�մ�С���������Ϊ��С�ֵ���
		calculate_distance_table();
		if (! is_no_path){
			if (algotype == BrutalForce)	brutal_force();
			else if (algotype == BranchAndBound)	branch_and_bound();
			organise_path_seg_by_seg();
			cout<<check_if_has_duplicate_nodes()<<endl;
		}
	}
/*	print_result("main: ",mainpath);*/
}
void Graph::dynamic_programming(){
	//����D[i][j],��¼��i��j����̾��룬��·�����������κ������ؾ��ڵ�
	initilize_dp_matrix();
	//����f[0][i] Ҳ���ǽڵ�i��dest�ľ���
	for (int i = 1 ; i < middlecnt+1; ++i){		Fij[0][i] = Dij[i][middlecnt+1];	Fij[0][i].mustpass.insert(mustpass_nodes[i]);}
	//�������·������Dij�ͳ�ʼF[0][j]��ʼ������F[i][j]
	//���ݵ���ʽ F[ksi][i] = min_j { D[i][j] + F[ksi -1][j]} (j =1...k)	
	//Ҳ���Ǵ�i��j����r���м�ڵ�(D[i][j])�����ϴ�j��dest�ľ���ksi -1���м�ڵ�(F[ksi -1][j])�����·��
	for (int ksi = 1 ; ksi < middlecnt; ++ksi){
		for (int i = 1; i < middlecnt + 1; ++i){
			min_distance_through_exactly_k_nodes(i, ksi);
		}
	}
	//�����Fij[middlecnt][0]��Ҳ���Ǵ�source��ʼ�����������м�ڵ�ﵽdest����̾���
	min_distance_through_exactly_k_nodes(0, middlecnt);
	path_trace_back();
}
void Graph::initilize_dp_matrix(){
	int i;
	Dij.resize(middlecnt+2);		Fij.resize(middlecnt+1);
	for ( i = 0 ; i < middlecnt+2; ++i){ Dij[i].resize(middlecnt+2);	}
	for ( i = 0 ; i < middlecnt+1; ++i){ Fij[i].resize(middlecnt+1);	}
	int start, end, current;
	list<int> path;						//ֻ��¼·�����м�ڵ�
	//����Dij���󣬼�¼��i��j����̾��룬��·�����������κ������ؾ��ڵ�
	for ( i = 0 ; i < middlecnt+2; ++i){ 
		start = mustpass_nodes[i];
		if(inputHasWeight)	dijkstra_without_pass_other_mustpass_nodes(start);
		else bfs_without_pass_other_mustpass_nodes(start);
		for (int j = i+1; j < middlecnt+2; ++j){		//ֻ����������
			end = mustpass_nodes[j];
			if (parent[end] >= 0){
				current = parent[end];
				path.clear();
				while(current != start){	
					path.push_front(current);
					current = parent[current];	
				}
				Dij[i][j].set_data(distance[end],path);
				path.reverse();				//����ͼ���Գ�·��
				Dij[j][i].set_data(distance[end],path);
			}else{	Dij[i][j].is_has_path = false;	Dij[j][i].is_has_path = false;	}
		}
	}
}
bool Graph::check_if_has_duplicate_nodes(){
	set<int> check,duplicate;	//��������ظ��ͱ����ظ�
	for(list<int>::iterator itr = mainpath.begin(); itr != mainpath.end(); ++itr){
		if (check.find(*itr) == check.end())	check.insert(*itr);
		else duplicate.insert(*itr);
	}
	if (!duplicate.empty()){
		//����Щ�ظ��ڵ�����Ϊ�ؾ��ڵ�
		int dup_sizes = duplicate.size();
/*		print_info();*/
		mustpass_nodes.resize(mustpass_nodes.size() + dup_sizes);
		copy(duplicate.begin(),duplicate.end(),mustpass_nodes.end()-dup_sizes-1);
		mustpass_nodes[mustpass_nodes.size()-1] = dest;
		for (set<int>::iterator itr = duplicate.begin(); itr != duplicate.end(); ++itr){
			allmustpass.insert(*itr);	middlenodes.insert(*itr);
		}
		middlecnt += dup_sizes;
/*		print_info();*/
/*		print_result("dup: ",mainpath);*/
		return true;
	}
	else return false;
}
void Graph::path_trace_back(){
	int next_node_idx = 0, now_node_idx = 0, start, end;
	mainpath.clear();
	if(Fij[middlecnt][now_node_idx].is_has_path){
		for (int ksi = middlecnt ; ksi > 0; --ksi){
			next_node_idx = Fij[ksi][now_node_idx].pre_node_index;
			start = mustpass_nodes[now_node_idx];	end = mustpass_nodes[Fij[ksi][now_node_idx].pre_node_index];
			mainpath.push_back(start);
			mainpath.insert(mainpath.end(), Dij[now_node_idx][next_node_idx].path.begin(),Dij[now_node_idx][next_node_idx].path.end());
			now_node_idx = next_node_idx;
		}
		next_node_idx = middlecnt + 1;		start = mustpass_nodes[now_node_idx];
		mainpath.push_back(start);
		mainpath.insert(mainpath.end(), Dij[now_node_idx][next_node_idx].path.begin(),Dij[now_node_idx][next_node_idx].path.end());
		mainpath.push_back(dest);
	}
}
void Graph::min_distance_through_exactly_k_nodes(int i,int ksi){
	set<int> union_mustpass;			//�м侭�����м�ڵ�
	int min_distance , tmp_distance, min_index;
	min_distance = POSTIVE_MAXINT; min_index =-1;
	for (int j = 1; j < middlecnt + 1; ++j){
		//û��·��������֮ǰ��·��������ǰ�ؾ��ڵ㣬������һ��
		if( ( i == j)  || (!Fij[ksi-1][j].is_has_path) ||  (!Dij[i][j].is_has_path) 
			||(Fij[ksi-1][j].mustpass.find(mustpass_nodes[i]) != Fij[ksi-1][j].mustpass.end())) continue;		
		tmp_distance = Dij[i][j].distance + Fij[ksi-1][j].distance;
		if (tmp_distance < min_distance){
			min_distance = tmp_distance; min_index = j;
		}
	}
	if (min_distance < POSTIVE_MAXINT){	//����·������¼����·��
		Fij[ksi][i].distance = min_distance;	 Fij[ksi][i].pre_node_index = min_index;	Fij[ksi][i].is_has_path = true;
		Fij[ksi][i].mustpass = Fij[ksi-1][min_index].mustpass;	Fij[ksi][i].mustpass.insert(mustpass_nodes[i]);		//ÿ�ζ�Ҫͨ�����Լ�
	}else{	Fij[ksi][i].is_has_path = false;	}
}
void Graph::branch_and_bound(){
	shortest_distance = POSTIVE_MAXINT;	
	bool is_prune = false;
	int tmp_shortest = 0, start , end ;
	greedy_method_for_upbound();		//̰�ķ����һ�����н���Ϊ�Ͻ磬Ϊ��֦��׼��
	do {
/*		print_info();*/
		tmp_shortest = 0;
		for (unsigned int i = 0 ; i < mustpass_nodes.size()-1; ++ i)	{		//�������ϵĳ���
			tmp_shortest += distancetable[mustpass_nodes[i]][mustpass_nodes[i+1]];
			//�м��������ؾ��ڵ� ���� Ŀǰ�ľ����Ѿ����ڵ�ǰ���ţ����Լ�֦
			if ((to_prune.find(pair<int,int>(mustpass_nodes[i], mustpass_nodes[i+1])) != to_prune.end()) ||
				tmp_shortest > shortest_distance ){			
//				cout<<"prune for exist mustpass nodes between: "<<mustpass_nodes[i]<<" "<<mustpass_nodes[i+1]<<endl;
//				cout<<"prune for current dist greater than best: "<<mustpass_nodes[i]<<" "<<tmp_shortest<<endl;
				start = i+2; end = middlecnt+1;
				if(end - start > 1){		//���������ϵ���������ֵ���
					insert_sort(mustpass_nodes, i+2, middlecnt+1);
					reverse(mustpass_nodes.begin()+i+2, mustpass_nodes.end()-1);
				}
				is_prune = true;		break;	
			}
		}
		if (is_prune){	is_prune = false;	continue;}
		if (tmp_shortest < shortest_distance){		//�ȵ�ǰ��������
			shortest_distance = tmp_shortest;		best_order = mustpass_nodes;
		}
	} while (next_permutation(mustpass_nodes.begin()+1,mustpass_nodes.end()-1));	//�м�ڵ�˳��仯��Դ�ڵ��Ŀ�Ľڵ�λ�ò���
}
void Graph::brutal_force(){
	shortest_distance = POSTIVE_MAXINT;	
	int tmp_shortest = 0;
	
	do {
		tmp_shortest = 0;
		for (unsigned int i = 0 ; i < mustpass_nodes.size()-1; ++ i)	{
			tmp_shortest += distancetable[mustpass_nodes[i]][mustpass_nodes[i+1]];
		}
		if (tmp_shortest < shortest_distance){			//�ȵ�ǰ��������
			shortest_distance = tmp_shortest;		best_order = mustpass_nodes;
		}
/*		print_info();*/
	} while (next_permutation(mustpass_nodes.begin()+1,mustpass_nodes.end()-1));	//�м�ڵ�˳��仯��Դ�ڵ��Ŀ�Ľڵ�λ�ò���
}
void Graph::greedy_method_for_upbound(){
	set<int> mustpass(middlenodes);
	best_order = mustpass_nodes; shortest_distance = 0;
	int tmp_distance, current=source, nextnode, index = 1;
	while (!mustpass.empty()){
		tmp_distance = POSTIVE_MAXINT;
		for ( set<int>::iterator itr = mustpass.begin() ; itr != mustpass.end() ; ++ itr ){
			if (distancetable[current][*itr] < tmp_distance)	{
				tmp_distance = distancetable[current][*itr];	nextnode = *itr;
			}
			
		}
		mustpass.erase(nextnode);
		best_order[index++] = current = nextnode;	
		shortest_distance += tmp_distance;
	}
	shortest_distance += distancetable[current][dest];
}
void Graph::calculate_distance_table(){
	for (unsigned int i = 0 ; i < mustpass_nodes.size(); ++ i)	{
		int start = mustpass_nodes[i], end , current ;
		if (inputHasWeight == true)	dijkstra_shortest_path(start,false);
		else bfs_shortest_path(start, false);		//ȫ�����㣬������������
		for (unsigned int j = 0; j< mustpass_nodes.size(); ++j) {
			if (distance[mustpass_nodes[j]] < 0){	is_no_path = true;	return;	}		//����ͨ�϶������ڸ�������·��
			end = mustpass_nodes[j], current = parent[end];		
			while(current != start){	//�ڵ��<i,j>�����·���а����бؾ��ڵ�,��Ҫ��֦
				if (middlenodes.find(current) != middlenodes.end()){/*	cout<<"to prune: "<<start<<" "<<end<<endl;*/
					to_prune.insert(pair<int,int>(start, end)); break;}
				current = parent[current];	
			}
		}
		if (! is_no_path){
			distancetable[mustpass_nodes[i]] = distance;		parenttable[mustpass_nodes[i]] = parent;
		}else break;
	}
}
void Graph::organise_path_seg_by_seg(){
	if (best_order.empty())	return;
	//������ô��򣬽�ÿ������·��ƴ�������������·��
	for (unsigned int i = 0 ; i < best_order.size()-1; ++ i)	{
		//����parent������ô�start->end�����·��
		vector<int> tmppath, &parent_vector = parenttable[best_order[i]]; 
		int start = best_order[i], end = best_order[i+1], current = end;
		while(current != start){	tmppath.push_back(current);	current = parent_vector[current];	}
		tmppath.push_back(start);
		reverse(tmppath.begin(),tmppath.end());

		if(i > 0)	mainpath.insert(mainpath.end(),tmppath.begin()+1,tmppath.end());
		else	mainpath.insert(mainpath.end(),tmppath.begin(),tmppath.end());	
	}
}
void Graph::insert_sort(vector<int> &array,int start,int end){
	int key , pos;
	for(int i = start+1; i < end; ++i){     //ǰ[0����i-1]�Ѿ������
		key = array[i];	pos = i-1;
		while((pos >=start) && (key < array[pos])){
			array[pos+1] = array[pos];
			--pos;
		}
		array[pos+1] = key;      //important
	}
}
void Graph::set_middle_nodes(vector<int> &middle){
	mustpass_nodes.resize(middle.size() + 2);
	mustpass_nodes[0] = source;
	middlecnt = middle.size();
	copy(middle.begin(),middle.end(),mustpass_nodes.begin()+1);
	mustpass_nodes[middlecnt+1] = dest;
	for (unsigned int i = 1 ; i < mustpass_nodes.size() -1; ++ i){	middlenodes.insert(mustpass_nodes[i]);	}
	allmustpass = middlenodes;	allmustpass.insert(source);	allmustpass.insert(dest);
}
void Graph::dijkstra_without_pass_other_mustpass_nodes(int root){
	//<length nodeid > tricky nodes[i]Ϊ�ڵ�i��heap�е�ָ�� 
	MinBinaryHeap<int, int > q;  //���ȶ���	
	vector <BinaryHeapNode<int, int >*> nodes(n);   	
	int min,w;		//min ��СȨֵ��Ӧ�Ľڵ�id, wΪ��min�����Ľڵ�id
	for (int i = 0; i < n; ++i){
		distance[i] = POSTIVE_MAXINT;     parent[i] = -1;
		if ( i == root ) { distance[i] = 0; parent[i] = i; }
		nodes[i] = q.insert(distance[i],i);
	}
	while(!q.empty()){
		min = q.minimum()->data();
//		if (min==end)	break;		//����ȵ�end�����ȶ��в��ܽ�������Ϊ�Ժ󶼲����ܶ����ɳ�
		if (q.minimum()->key() == POSTIVE_MAXINT){	break;	}	//����Ķ�������ɭ�֣�����Ҫ����
		q.removeMinimum();  nodes[min] = NULL;
		//��root���������ع��ڵ��Ϊ�ڶ���û�г���,�������ɳ�
		if ((min != root )&&(allmustpass.find(min) != allmustpass.end()))	continue;	
		//��ʼ���º�u�����ı� relax
		list<Node>::iterator itr = linktable[min].begin();
		while(itr !=  linktable[min].end()){
			w = itr->id;
			if (nodes[w] != NULL){   //δ��ѡ�м���S
				if (distance[min]+itr->weight < distance[w]){    //relax
					q.decreaseKey(nodes[w], distance[min]+itr->weight);
					distance[w] = distance[min]+itr->weight;
					parent[w] = min;
				}
			}
			++itr;
		}
	}
}
void Graph::dijkstra_shortest_path(int root, bool isTerminateAsFound){
	//<length nodeid > tricky nodes[i]Ϊ�ڵ�i��heap�е�ָ�� 
	MinBinaryHeap<int, int > q;  //���ȶ���	
	vector <BinaryHeapNode<int, int >*> nodes(n);   	
	int min,w;		//min ��СȨֵ��Ӧ�Ľڵ�id, wΪ��min�����Ľڵ�id
	for (int i = 0; i < n; ++i){
		distance[i] = POSTIVE_MAXINT;     parent[i] = -1;
		if ( i == root ) { distance[i] = 0; parent[i] = i; }
		nodes[i] = q.insert(distance[i],i);
	}
	while(!q.empty()){
		min = q.minimum()->data();
		if (isTerminateAsFound  && min==dest){	//����ȵ�dest�����ȶ��в��ܽ�������Ϊ�Ժ󶼲����ܶ����ɳ�
			break;
		}	
		if (q.minimum()->key() == POSTIVE_MAXINT){	break;	}	//����Ķ�������ɭ�֣�����Ҫ����
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
void Graph::bfs_without_pass_other_mustpass_nodes(int root){
	for (int i = 0; i< n; ++i){		parent[i] = -1;     distance[i] = -1;}
	int current = root ;  
	parent[current] = current;      distance[current] = 0;
	bool OuterLoopTerminate = false;
	queue<int> q;
	q.push(current);
	while (! q.empty()){
		current = q.front();    q.pop();
		//��root���������ع��ڵ��Ϊ�ڶ���û�г���,�������ɳ�
		if ((current != root )&&(allmustpass.find(current) != allmustpass.end()))	continue;
		//add every node w adjcent to v into stack
		list<Node>::iterator itr = linktable[current].begin();
		while(itr !=  linktable[current].end()){
			if (distance[itr->id] < 0){
				parent[itr->id] = current;
				distance[itr->id] = distance[current] + itr->weight;	
				q.push(itr->id);
			}
			++itr;
		}
	}
}
void Graph::bfs_shortest_path(int root, bool isTerminateAsFound){
	for (int i = 0; i< n; ++i){
		parent[i] = -1;     distance[i] = -1;
	}
	int current = root ;  
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
	is_no_path = false;
	resize(MAX_NODE_NUM);
}
void Graph::resize(int capacity){
	n = capacity;
	linktable.resize(n);
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
	n = max_node_id +1;
	distance.resize(n);
	parent.resize(n);
	ifs.close();
	return true;
}
void Graph::print_result(string outputfile){
	ofstream ofs(outputfile.c_str());
	ofs<<"main: ";
	if (mainpath.size() > 0){
		list<int>::iterator itr = mainpath.begin();
		ofs<<*itr;					++itr;
		for( ; itr != mainpath.end(); ++itr)
			ofs<<", "<<*itr;
	}
	ofs.close();
}

void Graph::print_result(string description, list<int> &path){
	cout<<description<<path.size()<<endl;
	for(list<int>::iterator itr = path.begin(); itr != path.end(); ++itr){
		cout<<*itr<<" ";
	}cout<<endl;
}
void Graph::print_info(){
	cout<<"shortest distance : "<<shortest_distance<<endl;
	for (unsigned int i = 0; i<mustpass_nodes.size() ; ++i)  {cout<<mustpass_nodes[i]<<" ";}cout<<endl;
}

void dp_record::set_data(int distance, list<int> &path){
	this->distance = distance;	this->path = path; is_has_path = true;
}
