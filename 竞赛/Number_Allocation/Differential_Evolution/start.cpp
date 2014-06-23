#include "crowd.h"

ofstream outfile("out.txt");

int main(){
	srand(time(0));

// 	chromosome a;
// 	int loop = 120;
// 	int choice = 14;
// 	for (int i =0;i<loop;i++)
// 	{
// 			a.allocate_all(choice);
// 			a.calculation_all_unit_violate_record();
// 
// 			cout<<a.total_vr<<"  ";
// 	
// 	}
// 	cout<<endl;
	//a.print_data();


	group a;
	a.evolution();
	a.print_best();

	//  	bruteforce_order a;
	//  	a.perm(4,matrix_size*matrix_size);
	// 	a.print_bestseq();


	outfile.close();

	return 0;
}