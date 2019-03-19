#include <iostream>
#include "SharedPtr.hpp"


int main()
{
	int *p = new int(2);
	SharedPtr<int, DFDef<int>> sp1(p);
	SharedPtr<int> sp2(sp1);
	FILE *fp = fopen("1.txt", "w");
	SharedPtr<FILE> sp3(fp);
	return 0;
}