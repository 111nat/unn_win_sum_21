#include "ScanTable.h"
#include "SortTable.h"
#include "TreeTable.h"
#include "HashTable.h"

int main()
{
	setlocale(LC_ALL, "rus");

	string path = "oldTable.txt";

	TTreeTable a(path);

	//cout << a.finder("Наумов Н.В.") << endl;

	//a.deleter("Захаров А.А.");
	//
	//cout << a.finder("Захаров А.А.") << endl;

	
	return 0;
	
}
