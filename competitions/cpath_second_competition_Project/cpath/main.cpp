#include <iostream>
#include <string>
#include <time.h>
#include <sstream>
#include "Graph.h"
using namespace std;

void test_pass_spefixed_nodes_samle();										//复赛测试样例
void get_middle_nodes(string para, vector<int> &middlenodes);	//从命令行获得中间节点
int main(int argc, char* argv[])
{
	if (argc > 1){
		string topofile,outputfile="result.txt";
		int source=1,dest=1;
		vector<int> middle_nodes;
		for (int i = 1; i < argc ; ++i)
		{
			string value(argv[i]);
			if (value.size() < 2)		continue;
			switch(value[1]){
			case 'f': topofile = value.substr(2,value.size());	break;
			case 'o': outputfile = value.substr(2,value.size());	break;
			case 's': source = atoi(value.substr(2,value.size()).c_str());	break;
			case 'd': dest = atoi(value.substr(2,value.size()).c_str());	break;
			case 'm': get_middle_nodes(value.substr(2,value.size()-1),middle_nodes);	break;
			default: break;
			}
		}
/*		cout<<topofile<<" "<<outputfile<<" "<<source<<" "<<dest<<endl;*/
		Graph graph;
		if (graph.read_inputfile(topofile))
		{
			time_t t_start,t_end;  
			t_start = clock();
			graph.source = source;	graph.dest = dest;
			graph.set_middle_nodes(middle_nodes);
			graph.calculate_path_pass_spefixed_nodes();
			t_end = clock();
			cout<<"run time: "<<t_end-t_start<<endl;
			graph.print_result(outputfile);
		}	
	}
	else{
		cout<<"参数不正确"<<endl;
		test_pass_spefixed_nodes_samle();
	}
	
	return 0;
}
void get_middle_nodes(string para, vector<int> &middlenodes){
/*	cout<<para<<endl;*/
	replace(para.begin(),para.end(),',',' ');		//将逗号替换为空格号
	int node_id;	stringstream ss;	ss<<para;
	while (ss>>node_id){
/*		cout<<node_id<<endl;*/
		middlenodes.push_back(node_id);
	}
}
void test_pass_spefixed_nodes_samle(){
	Graph graph;
	vector<int> middle_nodes;
// 	graph.read_inputfile("sample.txt");	graph.source = 20 ;	graph.dest = 32;
// 	middle_nodes.push_back(40);	middle_nodes.push_back(23);	middle_nodes.push_back(22);
// 	graph.read_inputfile("same_node_in_segs.txt");	graph.source = 0 ;	graph.dest = 2;
// 	middle_nodes.push_back(1);	/*middle_nodes.push_back(3);	*/
// 	graph.read_inputfile("dp_test.txt");	graph.source = 0 ;	graph.dest = 9;
// 	middle_nodes.push_back(2);	middle_nodes.push_back(4);	middle_nodes.push_back(6);	 middle_nodes.push_back(8);	
	graph.read_inputfile("bigdata.txt");	graph.source = 631 ;	graph.dest = 1980;
	middle_nodes.push_back(725);	middle_nodes.push_back(1268);	middle_nodes.push_back(1358); middle_nodes.push_back(2450);	middle_nodes.push_back(2451);	middle_nodes.push_back(2542);
	middle_nodes.push_back(1924);	middle_nodes.push_back(1834);	middle_nodes.push_back(1835); middle_nodes.push_back(2914);	middle_nodes.push_back(2470);	middle_nodes.push_back(2471);
	middle_nodes.push_back(2562);	middle_nodes.push_back(2207);	middle_nodes.push_back(2298); middle_nodes.push_back(2389);	middle_nodes.push_back(3400);	middle_nodes.push_back(2962);
	middle_nodes.push_back(2518);
	graph.set_middle_nodes(middle_nodes);

	graph.calculate_path_pass_spefixed_nodes();
}