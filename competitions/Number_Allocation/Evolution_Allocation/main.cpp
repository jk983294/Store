#include "group.h"

ofstream outfile("out.txt");

int main(){
	srand(time(0));

	chromosome a;
	a.best_order();
	//a.normal_order();
	a.calculation_all_unit_violate_record();
	a.print_data();


// 	group a;
// 	a.evolution();
// 	a.print_best();

//  	bruteforce_order a;
//  	a.perm(4,matrix_size*matrix_size);
// 	a.print_bestseq();
	

	outfile.close();

	return 0;
}