#include <iostream>
#include "SharedPtr.hpp"
using namespace std;

int main()
{
	int *p = new int(2);
	SharedPtr<int> sp1(p);
	
	FILE *fp = fopen("1.txt", "w");
	SharedPtr<FILE, DFDef<FILE>> sp3(fp);

	int *p2 = (int*)malloc(20);
	SharedPtr<int, Free> sp2(p2);
	return 0;
}