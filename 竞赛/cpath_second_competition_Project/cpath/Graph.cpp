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
		insert_sort(mustpass_nodes,1,middlecnt+1);		//中间节点按照从小到大排序成为最小字典序
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
	//计算D[i][j],记录从i到j的最短距离，该路径不允许经过任何其他必经节点
	initilize_dp_matrix();
	//计算f[0][i] 也就是节点i到dest的距离
	for (int i = 1 ; i < middlecnt+1; ++i){		Fij[0][i] = Dij[i][middlecnt+1];	Fij[0][i].mustpass.insert(mustpass_nodes[i]);}
	//根据最短路径矩阵Dij和初始F[0][j]开始迭代求F[i][j]
	//根据递推式 F[ksi][i] = min_j { D[i][j] + F[ksi -1][j]} (j =1...k)	
	//也就是从i到j经过r个中间节点(D[i][j])，加上从j到dest的经过ksi -1个中间节点(F[ksi -1][j])的最短路径
	for (int ksi = 1 ; ksi < middlecnt; ++ksi){
		for (int i = 1; i < middlecnt + 1; ++i){
			min_distance_through_exactly_k_nodes(i, ksi);
		}
	}
	//最后求Fij[middlecnt][0]，也就是从source开始，经过所有中间节点达到dest的最短距离
	min_distance_through_exactly_k_nodes(0, middlecnt);
	path_trace_back();
}
void Graph::initilize_dp_matrix(){
	int i;
	Dij.resize(middlecnt+2);		Fij.resize(middlecnt+1);
	for ( i = 0 ; i < middlecnt+2; ++i){ Dij[i].resize(middlecnt+2);	}
	for ( i = 0 ; i < middlecnt+1; ++i){ Fij[i].resize(middlecnt+1);	}
	int start, end, current;
	list<int> path;						//只记录路径的中间节点
	//计算Dij矩阵，记录从i到j的最短距离，该路径不允许经过任何其他必经节点
	for ( i = 0 ; i < middlecnt+2; ++i){ 
		start = mustpass_nodes[i];
		if(inputHasWeight)	dijkstra_without_pass_other_mustpass_nodes(start);
		else bfs_without_pass_other_mustpass_nodes(start);
		for (int j = i+1; j < middlecnt+2; ++j){		//只计算上三角
			end = mustpass_nodes[j];
			if (parent[end] >= 0){
				current = parent[end];
				path.clear();
				while(current != start){	
					path.push_front(current);
					current = parent[current];	
				}
				Dij[i][j].set_data(distance[end],path);
				path.reverse();				//无向图，对称路径
				Dij[j][i].set_data(distance[end],path);
			}else{	Dij[i][j].is_has_path = false;	Dij[j][i].is_has_path = false;	}
		}
	}
}
bool Graph::check_if_has_duplicate_nodes(){
	set<int> check,duplicate;	//用来检测重复和保存重复
	for(list<int>::iterator itr = mainpath.begin(); itr != mainpath.end(); ++itr){
		if (check.find(*itr) == check.end())	check.insert(*itr);
		else duplicate.insert(*itr);
	}
	if (!duplicate.empty()){
		//将这些重复节点设置为必经节点
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
	set<int> union_mustpass;			//中间经过的中间节点
	int min_distance , tmp_distance, min_index;
	min_distance = POSTIVE_MAXINT; min_index =-1;
	for (int j = 1; j < middlecnt + 1; ++j){
		//没有路径，或者之前的路径包含当前必经节点，搜索下一条
		if( ( i == j)  || (!Fij[ksi-1][j].is_has_path) ||  (!Dij[i][j].is_has_path) 
			||(Fij[ksi-1][j].mustpass.find(mustpass_nodes[i]) != Fij[ksi-1][j].mustpass.end())) continue;		
		tmp_distance = Dij[i][j].distance + Fij[ksi-1][j].distance;
		if (tmp_distance < min_distance){
			min_distance = tmp_distance; min_index = j;
		}
	}
	if (min_distance < POSTIVE_MAXINT){	//存在路径，记录该条路径
		Fij[ksi][i].distance = min_distance;	 Fij[ksi][i].pre_node_index = min_index;	Fij[ksi][i].is_has_path = true;
		Fij[ksi][i].mustpass = Fij[ksi-1][min_index].mustpass;	Fij[ksi][i].mustpass.insert(mustpass_nodes[i]);		//每次都要通过它自己
	}else{	Fij[ksi][i].is_has_path = false;	}
}
void Graph::branch_and_bound(){
	shortest_distance = POSTIVE_MAXINT;	
	bool is_prune = false;
	int tmp_shortest = 0, start , end ;
	greedy_method_for_upbound();		//贪心发求出一个可行解作为上界，为剪枝做准备
	do {
/*		print_info();*/
		tmp_shortest = 0;
		for (unsigned int i = 0 ; i < mustpass_nodes.size()-1; ++ i)	{		//计算该组合的长度
			tmp_shortest += distancetable[mustpass_nodes[i]][mustpass_nodes[i+1]];
			//中间有其他必经节点 或者 目前的距离已经大于当前最优，可以剪枝
			if ((to_prune.find(pair<int,int>(mustpass_nodes[i], mustpass_nodes[i+1])) != to_prune.end()) ||
				tmp_shortest > shortest_distance ){			
//				cout<<"prune for exist mustpass nodes between: "<<mustpass_nodes[i]<<" "<<mustpass_nodes[i+1]<<endl;
//				cout<<"prune for current dist greater than best: "<<mustpass_nodes[i]<<" "<<tmp_shortest<<endl;
				start = i+2; end = middlecnt+1;
				if(end - start > 1){		//将后面的组合调整到最大字典序
					insert_sort(mustpass_nodes, i+2, middlecnt+1);
					reverse(mustpass_nodes.begin()+i+2, mustpass_nodes.end()-1);
				}
				is_prune = true;		break;	
			}
		}
		if (is_prune){	is_prune = false;	continue;}
		if (tmp_shortest < shortest_distance){		//比当前最好则更新
			shortest_distance = tmp_shortest;		best_order = mustpass_nodes;
		}
	} while (next_permutation(mustpass_nodes.begin()+1,mustpass_nodes.end()-1));	//中间节点顺序变化，源节点和目的节点位置不变
}
void Graph::brutal_force(){
	shortest_distance = POSTIVE_MAXINT;	
	int tmp_shortest = 0;
	
	do {
		tmp_shortest = 0;
		for (unsigned int i = 0 ; i < mustpass_nodes.size()-1; ++ i)	{
			tmp_shortest += distancetable[mustpass_nodes[i]][mustpass_nodes[i+1]];
		}
		if (tmp_shortest < shortest_distance){			//比当前最好则更新
			shortest_distance = tmp_shortest;		best_order = mustpass_nodes;
		}
/*		print_info();*/
	} while (next_permutation(mustpass_nodes.begin()+1,mustpass_nodes.end()-1));	//中间节点顺序变化，源节点和目的节点位置不变
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
		else bfs_shortest_path(start, false);		//全部计算，将计算结果保存
		for (unsigned int j = 0; j< mustpass_nodes.size(); ++j) {
			if (distance[mustpass_nodes[j]] < 0){	is_no_path = true;	return;	}		//不连通肯定不存在复赛的主路径
			end = mustpass_nodes[j], current = parent[end];		
			while(current != start){	//节点对<i,j>的最短路径中包含有必经节点,需要剪枝
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
	//根据最好次序，将每段最优路径拼接起来获得最优路径
	for (unsigned int i = 0 ; i < best_order.size()-1; ++ i)	{
		//根据parent向量获得从start->end的最短路径
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
	for(int i = start+1; i < end; ++i){     //前[0……i-1]已经排序好
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
	//<length nodeid > tricky nodes[i]为节点i在heap中的指针 
	MinBinaryHeap<int, int > q;  //优先队列	
	vector <BinaryHeapNode<int, int >*> nodes(n);   	
	int min,w;		//min 最小权值对应的节点id, w为与min相连的节点id
	for (int i = 0; i < n; ++i){
		distance[i] = POSTIVE_MAXINT;     parent[i] = -1;
		if ( i == root ) { distance[i] = 0; parent[i] = i; }
		nodes[i] = q.insert(distance[i],i);
	}
	while(!q.empty()){
		min = q.minimum()->data();
//		if (min==end)	break;		//必须等到end出优先队列才能结束，因为以后都不可能对其松弛
		if (q.minimum()->key() == POSTIVE_MAXINT){	break;	}	//后面的都是其他森林，不需要遍历
		q.removeMinimum();  nodes[min] = NULL;
		//除root以外其他必过节点成为黑洞，没有出边,不帮助松弛
		if ((min != root )&&(allmustpass.find(min) != allmustpass.end()))	continue;	
		//开始更新和u相连的边 relax
		list<Node>::iterator itr = linktable[min].begin();
		while(itr !=  linktable[min].end()){
			w = itr->id;
			if (nodes[w] != NULL){   //未被选中加入S
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
	//<length nodeid > tricky nodes[i]为节点i在heap中的指针 
	MinBinaryHeap<int, int > q;  //优先队列	
	vector <BinaryHeapNode<int, int >*> nodes(n);   	
	int min,w;		//min 最小权值对应的节点id, w为与min相连的节点id
	for (int i = 0; i < n; ++i){
		distance[i] = POSTIVE_MAXINT;     parent[i] = -1;
		if ( i == root ) { distance[i] = 0; parent[i] = i; }
		nodes[i] = q.insert(distance[i],i);
	}
	while(!q.empty()){
		min = q.minimum()->data();
		if (isTerminateAsFound  && min==dest){	//必须等到dest出优先队列才能结束，因为以后都不可能对其松弛
			break;
		}	
		if (q.minimum()->key() == POSTIVE_MAXINT){	break;	}	//后面的都是其他森林，不需要遍历
		q.removeMinimum();  nodes[min] = NULL;
		//开始更新和u相连的边 relax
		list<Node>::iterator itr = linktable[min].begin();
		while(itr !=  linktable[min].end()){
			w = itr->id;
			if (nodes[w] != NULL)   //未被选中加入S
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
		//除root以外其他必过节点成为黑洞，没有出边,不帮助松弛
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
				//无需等到dest出队列才结束，因为以后都不会对dest松弛
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
	linktable[v].push_front(tmpnode);  //插入到邻接表
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
		cout<<"找不到指定拓扑文件"<<endl;
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
