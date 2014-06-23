#include <iostream>
#include <string>
#include <time.h>
#include "Graph.h"
using namespace std;

//ZTE forum's sample
void test_samle(){
	Graph graph;
	//sample	edgeTest	exception
/* 	graph.read_inputfile("nodeTest.txt");	graph.source = 1;	graph.dest = 4;*/
/*	graph.read_inputfile("nodeTest_noweight.txt");	graph.source = 1;	graph.dest = 4;*/
 	graph.read_inputfile("exception.txt");	graph.source = 1;	graph.dest = 4;
/* 	graph.read_inputfile("edgeTest.txt");	graph.source = 1;	graph.dest = 5;*/
/*	graph.read_inputfile("sample.txt");	graph.source = 20;	graph.dest = 32;*/
/*	graph.read_inputfile("bigdata.txt");	graph.source = 631;	graph.dest = 1980;*/
	graph.constraintype = EdgeDisjoint;     //NodeDisjoint EdgeDisjoint
	graph.calculate_all();
	graph.print_result("main: ",graph.primary_path);
	graph.print_result("backup: ",graph.backup_path);
}
void validate_correctness(){
	Graph graph;
	graph.read_inputfile("bigdata.txt");	graph.source = 631;	graph.dest = 1980;
	graph.constraintype = NodeDisjoint;     //NodeDisjoint EdgeDisjoint
	//maxflow_bigdata_edgeresult maxflow_bigdata_noderesult 
	//optimal_bigdata_edgeresult optimal_bigdata_noderesult
	//naive_bigdata_edgeresult naive_bigdata_noderesult
	graph.validate_correctness("naive_bigdata_noderesult.txt");	
}
int main(int argc, char* argv[])
{
	if (argc > 1)
	{
		string topofile,outputfile="result.txt";
		int source=1,dest=1;
		ConstrainTpye constrain = EdgeDisjoint;
		for (int i = 1; i < argc ; ++i)
		{
			string value(argv[i]);
			if (value.size() < 2)		continue;
			switch(value[1]){
			case 'f': topofile = value.substr(2,value.size());	break;
			case 'o': outputfile = value.substr(2,value.size());	break;
			case 's': source = atoi(value.substr(2,value.size()).c_str());	break;
			case 'd': dest = atoi(value.substr(2,value.size()).c_str());	break;
			case 'c': 
				if(atoi(value.substr(2,value.size()).c_str()) == 1)	constrain = NodeDisjoint;
				else constrain = EdgeDisjoint;
				break;
			}
		}
/*		cout<<topofile<<" "<<outputfile<<" "<<source<<" "<<dest<<" "<<constrain<<endl;*/
		Graph graph;
		if (graph.read_inputfile(topofile))
		{
			time_t t_start,t_end;  
			t_start = clock();
			graph.source = source;	graph.dest = dest;
			graph.constraintype = constrain;
			graph.calculate_all();
			t_end = clock();
			cout<<"run time: "<<t_end-t_start<<endl;
			graph.print_result(outputfile);
		}	
	}
	else{
		cout<<"参数不正确"<<endl;
/*		test_samle();*/
/*		validate_correctness();*/
	}
	
	return 0;
}
