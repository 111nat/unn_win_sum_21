#pragma once
#include "ScanTable.h"

int Hash(string fullName)
{
	if (fullName.length() != 3)
	{
		cout << "Ошибка хэш (длина)" << endl;
		return int();
	}
	for (int i = 0; i < fullName.length(); i++)
	{
		if (fullName[i] > 'Я' || fullName[i] < 'А')
		{
			cout << "Ошибка хэш (не те символы)" << endl;
			return int();
		}
	}
	int hash;
	hash = (fullName[0] - 'А') % 256 + (fullName[1] - 'А') % 256 * 32 + (fullName[2] - 'А') % 256 * 32 * 32;
	return hash;
}

class THashTable : public TScanTable
{
	vector<list<string>> hashTable;
public:
	THashTable(string path) : TScanTable(path)
	{
		hashTable.reserve(Hash("ЯЯЯ") + 1);

		for (int i = 0; i < hashTable.capacity(); i++)
		{
			hashTable.push_back(list<string>());
		}

		string subjects = array[0];
		for (int i = 0; i < subjects.length(); i++)
		{
			if (subjects[i] == '|')
			{
				subjects[i] = '\t';
			}
		}
		subjects += '\n';

		string person;
		string fullName;

		for (int i = 1; i < stringCount; i++)
		{
			person = subjects;
			fullName = string();
			for (int j = 0; j < array[i].length(); j++)
			{
				if (array[i][j] == '|')
				{
					person.push_back('\t');
					continue;
				}
				if (array[i][j] >= 'А' && array[i][j] <= 'Я')
				{
					fullName.push_back(array[i][j]);
				}
				person.push_back(array[i][j]);
			}

			hashTable[Hash(fullName)].push_front(person);
		}

	}

	string finder(string key) override
	{
		int goNext = 0;
		if (key[0] >= '0' && key[0] <= '9')
		{
			string help = string();
			help.push_back(key[0]);
			goNext = stoi(help);
			key.erase(0, 1);
		}

		string new_key = string();
		for (int i = 0; i < key.length(); i++)
		{
			if (key[i] >= 'А' && key[i] <= 'Я')
			{
				new_key.push_back(key[i]);
			}
		}

		int index = Hash(new_key);
		if (hashTable[index].size() == 0)
		{
			cout << "Нет такого ФИО" << endl;
			return string();
		}
		int index_in_list = 0;
		while (index_in_list < hashTable[index].size())
		{
			if ((*next(hashTable[index].begin(), index_in_list)).find(key) != string::npos)
			{
				if (goNext > 0)
				{
					goNext--;
					index_in_list++;
					continue;
				}
				cout << *next(hashTable[index].begin(), index_in_list) << endl;
				return *next(hashTable[index].begin(), index_in_list);
			}
			index_in_list++;
		}
		cout << "Нет такого ФИО" << endl;
		return string();
	}

	bool deleter(string key) override
	{
		int goNext = 0;
		if (key[0] >= '0' && key[0] <= '9')
		{
			string help = string();
			help.push_back(key[0]);
			goNext = stoi(help);
			key.erase(0, 1);
		}

		string new_key = string();
		for (int i = 0; i < key.length(); i++)
		{
			if (key[i] >= 'А' && key[i] <= 'Я')
			{
				new_key.push_back(key[i]);
			}
		}

		int index = Hash(new_key);
		if (hashTable[index].size() == 0)
		{
			cout << "Нет такого ФИО" << endl;
			return false;
		}
		int index_in_list = 0;
		while (index_in_list < hashTable[index].size())
		{
			if ((*next(hashTable[index].begin(), index_in_list)).find(key) != string::npos)
			{
				if (goNext > 0)
				{
					goNext--;
					index_in_list++;
					continue;
				}
				hashTable[index].erase(next(hashTable[index].begin(), index_in_list));
				TScanTable::deleter(key);
				return true;
			}
			index_in_list++;
		}
		cout << "Нет такого ФИО" << endl;
		return false;
	}

	void inserter(string person, string marks) override
	{
		int number_of_marks = 0;
		for (int i = 0; i < array[0].length(); i++)
		{
			if (array[0][i] == '|')
			{
				number_of_marks++;
			}
		}

		if (marks.length() / 2 + 1 != number_of_marks)
		{
			cout << "Ошибка: неправильное кол-во оценок. Кол-во = " << number_of_marks << endl;
			return void();
		}

		string subjects = array[0];
		for (int i = 0; i < subjects.length(); i++)
		{
			if (subjects[i] == '|')
			{
				subjects[i] = '\t';
			}
		}
		subjects += '\n';

		string fullName = string();
		for (int i = 0; i < person.length(); i++)
		{
			if (person[i] >= 'А' && person[i] <= 'Я')
			{
				fullName.push_back(person[i]);
			}
		}

		if (fullName.length() != 3)
		{
			cout << "Неправильное ФИО" << endl;
			return void();
		}

		string truePerson = subjects + person;
		truePerson += '\t';
		for (int i = 0; i < marks.length(); i++)
		{
			if (marks[i] == ' ')
			{
				truePerson.push_back('\t');
				continue;
			}
			truePerson.push_back(marks[i]);
		}


		hashTable[Hash(fullName)].push_front(truePerson);

		string marks_to_scanTable = string();
		marks_to_scanTable.push_back('|');
		for (int i = 0; i < marks.length(); i++)
		{
			if (marks[i] == ' ')
			{
				marks_to_scanTable.push_back('|');
				continue;
			}
			marks_to_scanTable.push_back(marks[i]);
		}
		TScanTable::inserter(person + marks_to_scanTable, "line");
	}
};
