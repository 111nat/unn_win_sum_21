#pragma once
#include <stack>
#include <vector>
#include <queue>
#include <list>

#include "fileWorker.h"

//abstract class table
class Table
{
public:
	Table(string path);
	~Table();
	virtual void inserter(string str, string key) = 0;
	virtual string finder(string key) = 0;
	virtual bool deleter(string key) = 0;

protected:
	int stringCount;
	string* array;
};

Table::Table(string path)
{
	int ammountStrings = int();
	string table = fileWorker("read", path);
	for (int i = 0; i < table.length(); i++)
	{
		if (table[i] == '\n')
		{
			ammountStrings++;
		}
	}
	stringCount = ammountStrings;
	array = new string[ammountStrings];

	for (int i = 0, j = 0; i < table.length(); i++)
	{
		if (table[i] == '\n')
		{
			array[j].push_back(table[i]);
			i++;
			j++;
			if (j == ammountStrings)
			{
				break;
			}
		}
		array[j].push_back(table[i]);
	}
}

Table::~Table()
{
	delete[] array;
}
