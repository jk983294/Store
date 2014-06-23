#include <iostream>

using namespace std;

int main(){
	int index =0;
	float sum = 0;

	for (index = 6; index <= 100;index++)
	{
		sum += (float)1/index;
	}
	cout<<sum;
	return 0;
}