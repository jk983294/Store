// TestCompDll.cpp : Defines the entry point for the console application.
//

#include "../../CompDll/CompDll.h"

int main(int argc, char* argv[])
{

	int val = 10;
	Set(&val);
	Print();
	printf("val now is %d\n" , val);
	printf("Hello World!\n");
	Initialize(20);
	TestSender();

	int wait;
	scanf("d%" , &wait);
	return 0;
}

