#include "ScanTable.h"
#include "SortTable.h"
#include "TreeTable.h"
#include "HashTable.h"

int main()
{
	setlocale(LC_ALL, "rus");

	string path = "oldTable.txt";

	TTreeTable a(path);

	//cout << a.finder("������ �.�.") << endl;

	//a.deleter("������� �.�.");
	//
	//cout << a.finder("������� �.�.") << endl;

	
	return 0;
	
}
