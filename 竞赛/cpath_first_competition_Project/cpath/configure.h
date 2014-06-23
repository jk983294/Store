#ifndef CONFIGURE_H
#define CONFIGURE_H

enum ConstrainTpye{
	EdgeDisjoint ,
	NodeDisjoint 
};
enum AlgorithmType{
	Naive,
	Optimal,
	MaxFlow
};


const AlgorithmType algotype = MaxFlow;
const bool inputHasWeight = false;
const int POSTIVE_MAXINT = 0x7fff;
const int MAX_NODE_NUM = 5000;


#endif