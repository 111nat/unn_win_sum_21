#include <iostream>
#include <fstream>
#include <Windows.h>
#include <string>
#include <stack>
#include <vector>
#include <queue>
#include <list>
using namespace std;

//for work with file
string fileWorker(string state, string path, string newTable = string())
{
	fstream file;
	if (state == "read")
	{
		string text = string();
		char textHelper = char();
		file.open(path, fstream::in);
		if (!file.is_open())
		{
			cout << "Ошибка открытия файла" << endl;
		}
		else
		{
			cout << "Файл открыт для чтения" << endl;
			while (file.get(textHelper))
			{
				//SetConsoleCP(1251);
				//SetConsoleCP(866);
				text.push_back(textHelper);
			}

		}
		file.close();
		return text;
	}
	else if (state == "write")
	{
		file.open(path, fstream::out);
		if (!file.is_open())
		{
			cout << "Ошибка открытия файла" << endl;
		}
		else
		{
			cout << "Файл открыт для записи" << endl;
			file << newTable;

		}
		file.close();
		return string();
	}
	else
	{
		cout << "Ошибка функции fileWorker" << endl;
		return string();
	}
}

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
	//idPupil = new int[ammountStrings - 1];

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
	
	/*string newTable = string();
	for (int i = 0; i < stringCount; i++)
	{
		newTable = newTable + array[i];
	}
	fileWorker("write", "newTable.txt", newTable);*/
	delete[] array;
	//delete[] idPupil;
}

//not sort table
class TScanTable : public Table
{
protected:
	string** tableMatrix = new string * [stringCount];
	int ammountTableColumns;
	stack<int> deleted_column;
	stack<int> deleted_line;
public:
	TScanTable(string path) : Table(path) 
	{
		string pieceOfString = string();
		int column = 1;
		for (int j = 0; j < array[0].length(); j++)
		{
			if (array[0][j] == '|')
			{
				column++;
			}
		}

		ammountTableColumns = column;

		for (int i = 0; i < stringCount; i++)
		{
			tableMatrix[i] = new string[column];
		}

		for (int i = 0, endOfString = 0; i < stringCount; i++, endOfString = 0)
		{
			for (int j = 0; j < column; j++)
			{
				while (array[i][endOfString] != '|' && array[i][endOfString] != '\n')
				{
					tableMatrix[i][j].push_back(array[i][endOfString]);
					endOfString++;
				}
				endOfString++;
				
			}
		}
	};

	~TScanTable()
	{
		string newTable = string();
		for (int i = 0; i < stringCount; i++)
		{
			if (tableMatrix[i][0] == "deleted")
			{
				continue;
			}
			for (int j = 0; j < ammountTableColumns; j++)
			{
				if (tableMatrix[0][j] == "deleted")
				{
					continue;
				}

				newTable = newTable + tableMatrix[i][j];
				
				if (j == ammountTableColumns - 1)
				{
					break;
				}
				newTable += "|";
			}
			newTable += "\n";
		}

		fileWorker("write", "newTable.txt", newTable);

		for (int i = 0; i < stringCount; i++)
		{
			delete[] tableMatrix[i];
		}
		delete[] tableMatrix;
	}

	void inserter(string str, string key = "-1") override
	{
		if (key == "column")
		{
			if (!deleted_column.empty())
			{
				int column_number = deleted_column.top();
				deleted_column.pop();
				for (int i = 0, j = 0; i < stringCount; i++)
				{
					if (tableMatrix[i][0] == "deleted")
					{
						continue;
					}
					string helper = string();
					for (; j < str.length(); j++)
					{
						if (str[j] == '|')
						{
							j++;
							break;
						}
						helper.push_back(str[j]);
					}
					tableMatrix[i][column_number] = helper;
				}
			}
			else
			{
				string** tableMatrixHelper = new string * [stringCount];
				ammountTableColumns++;

				for (int i = 0; i < stringCount; i++)
				{
					tableMatrixHelper[i] = new string[ammountTableColumns];
				}

				for (int i = 0; i < stringCount; i++)
				{
					for (int j = 0; j < ammountTableColumns - 1; j++)
					{
						tableMatrixHelper[i][j] = tableMatrix[i][j];
					}
				}

				for (int i = 0, j = 0; i < stringCount; i++)
				{
					if (tableMatrixHelper[i][0] == "deleted")
					{
						continue;
					}
					string helper = string();
					for (; j < str.length(); j++)
					{
						if (str[j] == '|')
						{
							j++;
							break;
						}
						helper.push_back(str[j]);
					}
					tableMatrixHelper[i][ammountTableColumns - 1] = helper;
				}
				tableMatrix = tableMatrixHelper;
			}
		}

		if (key == "line")
		{
			if (!deleted_line.empty())
			{
				int line_number = deleted_line.top();
				deleted_line.pop();
				for (int i = 0, j = 0; i < ammountTableColumns; i++)
				{
					if (tableMatrix[0][i] == "deleted")
					{
						continue;
					}
					string helper = string();
					for (; j < str.length(); j++)
					{
						if (str[j] == '|')
						{
							j++;
							break;
						}
						helper.push_back(str[j]);
					}
					tableMatrix[line_number][i] = helper;
				}
			}
			else
			{
				string** tableMatrixHelper = new string * [stringCount + 1];

				for (int i = 0; i < stringCount + 1; i++)
				{
					tableMatrixHelper[i] = new string[ammountTableColumns];
				}

				for (int i = 0; i < stringCount; i++)
				{
					for (int j = 0; j < ammountTableColumns; j++)
					{
						tableMatrixHelper[i][j] = tableMatrix[i][j];
					}
				}

				for (int i = 0, j = 0; i < ammountTableColumns; i++)
				{
					if (tableMatrixHelper[0][i] == "deleted")
					{
						continue;
					}
					string helper = string();
					for (; j < str.length(); j++)
					{
						if (str[j] == '|')
						{
							j++;
							break;
						}
						helper.push_back(str[j]);
					}
					tableMatrixHelper[stringCount][i] = helper;
				}
				tableMatrix = tableMatrixHelper;
				stringCount++;
			}
		}
	}

	string finder(string key) override
	{
		bool iskeyNumber = true;
		for (int i = 0; i < key.length(); i++)
		{
			if (key[i] < '0' || key[i] > '9')
			{
				iskeyNumber = false;
			}
		}

		if (iskeyNumber)
		{
			int keyNumber = stoi(key);

			while (tableMatrix[keyNumber][0] == "deleted")
			{
				keyNumber++;
				if (keyNumber == stringCount)
				{
					break;
				}
			}

			if (keyNumber < 1 || keyNumber > stringCount - 1)
			{
				cout << "Выход за массив" << endl;
				return string();
			}

			

			string returner = string();
			for (int i = 0; i < ammountTableColumns; i++)
			{
				returner = returner + tableMatrix[keyNumber][i];
				if (i + 1 == ammountTableColumns)
				{
					break;
				}
				returner = returner + " ";
			}
			cout << returner << endl;
			return returner;
		}
		else
		{
			for (int i = 1; i < stringCount; i++)
			{
				if (key == "deleted")
				{
					cout << "Слово не найдено" << endl;
					return string();
				}

				if (key == tableMatrix[i][0])
				{
					string returner = string();
					//------------
					for (int j = 0; j < ammountTableColumns; j++)
					{
						returner = returner + tableMatrix[0][j];
						returner += " ";
					}
					returner += "\n";
					//-----------
					for (int j = 0; j < ammountTableColumns; j++)
					{
						returner = returner + tableMatrix[i][j];
						if (j + 1 == stringCount)
						{
							break;
						}
						returner += "\t";
					}
					cout << returner << endl;
					return returner;
				}

			}

			for (int i = 0; i < ammountTableColumns; i++)
			{
				if (key == "deleted")
				{
					cout << "Слово не найдено" << endl;
					return string();
				}

				if (key == tableMatrix[0][i])
				{
					string returner = string();
					for (int j = 0; j < stringCount; j++)
					{
						//-----------
						if (i != 0)
						{
							returner = returner + tableMatrix[j][0];
							returner += "\t";
						}
						//----------
						returner = returner + tableMatrix[j][i];
						if (j + 1 == stringCount)
						{
							break;
						}
						returner = returner + "\n";
					}
					cout << returner << endl;
					return returner;
				}

			}


			cout << "Ошибка: слово не найдено" << endl;
		}

		return string();
	}

	bool deleter(string key) override
	{
		bool iskeyNumber = true;
		for (int i = 0; i < key.length(); i++)
		{
			if (key[i] < '0' || key[i] > '9')
			{
				iskeyNumber = false;
			}
		}

		if (iskeyNumber)
		{
			int keyNumber = stoi(key);
			if (keyNumber < 1 || keyNumber > stringCount - 1)
			{
				cout << "Выход за массив" << endl;
				return false;
			}
			tableMatrix[keyNumber][0] = "deleted";
			deleted_line.push(keyNumber);
			return true;
		}
		else
		{
			for (int i = 1; i < stringCount; i++)
			{
				if (key == tableMatrix[0][i])
				{
					tableMatrix[0][i] = "deleted";
					deleted_column.push(i);
					return true;
				}

				if (key == tableMatrix[i][0])
				{
					tableMatrix[i][0] = "deleted";
					deleted_line.push(i);
					return true;
				}

			}
			cout << "Ошибка: слово не найдено" << endl;
		}

		return false;
	}

	void printArray()
	{
		cout << "-----" << endl;
		for (int i = 0; i < stringCount; i++)
		{
			cout << array[i];
		}
		cout << "-----" << endl;
	}

	void printMatrix()
	{
		cout << "-----" << endl;
		for (int i = 0; i < stringCount; i++)
		{
			for (int j = 0; j < ammountTableColumns; j++)
			{
				cout << tableMatrix[i][j] << "    ";
			}
			cout << endl;
			
		}
		cout << "-----" << endl;
	}
};

class TSortTable : public TScanTable
{
	int binarySearch_line(string key, int min, int max)
	{
		if (min == max)
		{
			if (tableMatrix[max][0] == key)
			{
				return max;
			}
			else
			{
				return NULL;
			}
		}

		if (max < min)
		{
			cout << "max < min" << endl;
			return NULL;
		}

		int mid = (max - min) / 2 + min;//max / 2 + (max % 2 ? 1 : 0);
		if (tableMatrix[mid][0] < key)
		{
			return binarySearch_line(key, mid + 1, max);
		} 
		else if (tableMatrix[mid][0] > key)
		{
			return binarySearch_line(key, min, mid - 1);
		}
		else
		{
			return mid;
		}
	}

	int binarySearch_column(string key, int min, int max)
	{
		if (min == max)
		{
			if (tableMatrix[0][max] == key)
			{
				return max;
			}
			else
			{
				return NULL;
			}
		}

		if (max < min)
		{
			cout << "max < min" << endl;
			return NULL;
		}

		int mid = (max - min) / 2 + min;;
		if (tableMatrix[0][mid] < key)
		{
			return binarySearch_column(key, mid + 1, max);
		}
		else if (tableMatrix[0][mid] > key)
		{
			return binarySearch_column(key, min, mid - 1);
		}
		else
		{
			return mid;
		}
	}
public:
	TSortTable(string path) : TScanTable(path)
	{
		for (int i = 1; i < stringCount; i++)
		{
			for (int j = i + 1; j < stringCount; j++)
			{
				if (strcmp(tableMatrix[i][0].c_str(), tableMatrix[j][0].c_str()) > 0)
				{
					string* helper_arr = tableMatrix[i];
					tableMatrix[i] = tableMatrix[j];
					tableMatrix[j] = helper_arr;
				}
			}
		}

		for (int i = 1; i < ammountTableColumns; i++)
		{
			for (int j = i + 1; j < ammountTableColumns; j++)
			{
				if (strcmp(tableMatrix[0][i].c_str(), tableMatrix[0][j].c_str()) > 0)
				{
					for (int k = 0; k < stringCount; k++)
					{
						swap(tableMatrix[k][i], tableMatrix[k][j]);
					}
				}
			}
		}

	}

	bool deleter(string key) override
	{
		if (key[0] == '*')
		{
			for (int i = 0; i < key.length() ; i++)
			{
				key[i] = key[i + 1];
			}

			key.pop_back();

			int helper = binarySearch_column(key, 1, ammountTableColumns - 1);

			if (helper == NULL)
			{
				cout << "cant delete" << endl;
				return false;
			}
			
			tableMatrix[0][helper] = "deleted";
		}
		else
		{
			int helper = binarySearch_line(key, 1, stringCount - 1);

			if (helper == NULL)
			{
				cout << "cant delete" << endl;
				return false;
			}

			tableMatrix[helper][0] = "deleted";
		}
		return true;
	}

	string finder(string key) override
	{
		if (key[0] == '*')
		{
			for (int i = 0; i < key.length(); i++)
			{
				key[i] = key[i + 1];
			}

			key.pop_back();

			int helper = binarySearch_column(key, 1, ammountTableColumns - 1);

			if (helper == NULL)
			{
				cout << "cant find" << endl;
				return string();
			}

			string returner;
			for (int i = 0; i < stringCount; i++)
			{
				if (tableMatrix[i][0] == "deleted") continue;
				returner += tableMatrix[i][0];
				returner += "\t";
				returner += tableMatrix[i][helper];
				returner += "\n";
			}

			cout << returner << endl;
			return returner;
		}
		else
		{
			bool iskeyNumber = true;
			for (int i = 0; i < key.length(); i++)
			{
				if (key[i] < '0' || key[i] > '9')
				{
					iskeyNumber = false;
				}
			}

			if (iskeyNumber)
			{
				int keyNumber = stoi(key);

				while (tableMatrix[keyNumber][0] == "deleted")
				{
					keyNumber++;
					if (keyNumber == stringCount)
					{
						break;
					}
				}

				if (keyNumber < 1 || keyNumber > stringCount - 1)
				{
					cout << "Выход за массив" << endl;
					return string();
				}

				string returner;
				for (int i = 0; i < ammountTableColumns; i++)
				{
					if (tableMatrix[0][i] == "deleted") continue;
					returner += tableMatrix[0][i];
					returner += "\t";
				}

				returner += "\n";

				for (int i = 0; i < ammountTableColumns; i++)
				{
					if (tableMatrix[0][i] == "deleted") continue;
					returner += tableMatrix[keyNumber][i];
					returner += "\t";
				}
				cout << returner << endl;
				return returner;
			}

			int helper = binarySearch_line(key, 1, stringCount - 1);

			if (helper == NULL)
			{
				cout << "cant find" << endl;
				return string();
			}

			string returner;
			for (int i = 0; i < ammountTableColumns; i++)
			{
				if (tableMatrix[0][i] == "deleted") continue;
				returner += tableMatrix[0][i];
				returner += "\t";
			}

			returner += "\n";

			for (int i = 0; i < ammountTableColumns; i++)
			{
				if (tableMatrix[0][i] == "deleted") continue;
				returner += tableMatrix[helper][i];
				returner += "\t";
			}
			cout << returner << endl;
			return returner;
		}
		return string();
	}

	void inserter(string str, string key = "-1") override
	{
		if (key == "column")
		{
			if (!deleted_column.empty())
			{
				int column_number = deleted_column.top();
				deleted_column.pop();
				for (int i = 0, j = 0; i < stringCount; i++)
				{
					if (tableMatrix[i][0] == "deleted")
					{
						continue;
					}
					string helper = string();
					for (; j < str.length(); j++)
					{
						if (str[j] == '|')
						{
							j++;
							break;
						}
						helper.push_back(str[j]);
					}
					tableMatrix[i][column_number] = helper;
				}
			}
			else
			{
				string** tableMatrixHelper = new string * [stringCount];
				ammountTableColumns++;

				for (int i = 0; i < stringCount; i++)
				{
					tableMatrixHelper[i] = new string[ammountTableColumns];
				}

				for (int i = 0; i < stringCount; i++)
				{
					for (int j = 0; j < ammountTableColumns - 1; j++)
					{
						tableMatrixHelper[i][j] = tableMatrix[i][j];
					}
				}

				for (int i = 0, j = 0; i < stringCount; i++)
				{
					if (tableMatrixHelper[i][0] == "deleted")
					{
						continue;
					}
					string helper = string();
					for (; j < str.length(); j++)
					{
						if (str[j] == '|')
						{
							j++;
							break;
						}
						helper.push_back(str[j]);
					}
					tableMatrixHelper[i][ammountTableColumns - 1] = helper;
				}
				tableMatrix = tableMatrixHelper;
			}
		}

		if (key == "line")
		{
			if (!deleted_line.empty())
			{
				int line_number = deleted_line.top();
				deleted_line.pop();
				for (int i = 0, j = 0; i < ammountTableColumns; i++)
				{
					if (tableMatrix[0][i] == "deleted")
					{
						continue;
					}
					string helper = string();
					for (; j < str.length(); j++)
					{
						if (str[j] == '|')
						{
							j++;
							break;
						}
						helper.push_back(str[j]);
					}
					tableMatrix[line_number][i] = helper;
				}
			}
			else
			{
				string** tableMatrixHelper = new string * [stringCount + 1];

				for (int i = 0; i < stringCount + 1; i++)
				{
					tableMatrixHelper[i] = new string[ammountTableColumns];
				}

				for (int i = 0; i < stringCount; i++)
				{
					for (int j = 0; j < ammountTableColumns; j++)
					{
						tableMatrixHelper[i][j] = tableMatrix[i][j];
					}
				}

				for (int i = 0, j = 0; i < ammountTableColumns; i++)
				{
					if (tableMatrixHelper[0][i] == "deleted")
					{
						continue;
					}
					string helper = string();
					for (; j < str.length(); j++)
					{
						if (str[j] == '|')
						{
							j++;
							break;
						}
						helper.push_back(str[j]);
					}
					tableMatrixHelper[stringCount][i] = helper;
				}
				tableMatrix = tableMatrixHelper;
				stringCount++;
			}
		}

		for (int i = 1; i < stringCount; i++)
		{
			for (int j = i + 1; j < stringCount; j++)
			{
				if (strcmp(tableMatrix[i][0].c_str(), tableMatrix[j][0].c_str()) > 0)
				{
					string* helper_arr = tableMatrix[i];
					tableMatrix[i] = tableMatrix[j];
					tableMatrix[j] = helper_arr;
				}
			}
		}

		for (int i = 1; i < ammountTableColumns; i++)
		{
			for (int j = i + 1; j < ammountTableColumns; j++)
			{
				if (strcmp(tableMatrix[0][i].c_str(), tableMatrix[0][j].c_str()) > 0)
				{
					for (int k = 0; k < stringCount; k++)
					{
						swap(tableMatrix[k][i], tableMatrix[k][j]);
					}
				}
			}
		}
	}
};

//int middle(int a, int b)
//{
//	return (b - a) / 2 + a;
//}

vector<int> balanc(int length)
{
	bool len;
	if (length % 2)
	{
		len = true;
		length = length + 1;
	}
	else
	{
		len = false;
	}
	vector<int> returner;
	vector<int> prev;
	for (int i = 1; i < length; i++)
	{
		prev.push_back(i);
	}
	
	int mid = prev.size();
	bool tr = true;
	
	while (!prev.empty())
	{
		mid = prev.size() % 2 ? prev.size()/2 + 1 : prev.size()/2;
		if (prev.size() % 2 == 1)
		{
			while (prev.front() != prev.at(mid - 1))
			{
				if (tr)
				{
					returner.push_back(*(prev.begin() + mid - 1));
					tr = false;
					mid = (prev.size() / 2) % 2 ? prev.size() / 2 / 2 + 1 : prev.size() / 2 / 2;
					//prev.erase((prev.begin() + mid - 1));
				}
				else
				{
					returner.push_back(*(prev.begin() + mid - 1));
					returner.push_back(*(prev.begin() + prev.size() - 1 - mid + 1));
					mid = mid % 2 ? mid / 2 + 1 : mid / 2;
				}

			}
		}
		else
		{
			bool tr2 = true;

			while (prev.front() != prev.at(mid - 1))
			{
				if (tr2)
				{
					returner.push_back(*(prev.begin() + mid - 1 - 1));
					returner.push_back(*(prev.begin() + prev.size() - 1 - mid + 1 + 1));
					mid = mid % 2 ? mid / 2 + 1 : mid / 2;
					tr2 = false;
				}
				else
				{
					returner.push_back(*(prev.begin() + mid - 1));
					returner.push_back(*(prev.begin() + prev.size() - 1 - mid + 1));
					mid = mid % 2 ? mid / 2 + 1 : mid / 2;
				}
			}
		}
		
		if(*(prev.begin() + prev.size() - 1 - mid + 1) == returner.back())
		{ }
		else
		{
			returner.push_back(*(prev.begin() + mid - 1));
			returner.push_back(*(prev.begin() + prev.size() - 1 - mid + 1));
		}

		for (int i = 0; i < returner.size(); i++)
		{
			for (int j = 0; j < prev.size(); j++)
			{
				if (returner[i] == prev[j])
				{
					prev.erase(prev.begin() + j );
					break;
				}
			}
		}
	}

	if (len)
	{
		for (int i = 0; i < returner.size(); i++)
		{
			if (returner[i] == length - 1)
			{
				returner.erase(returner.begin() + i);
			}
		}
	}

	for (int i = 0; i < returner.size(); i++)
	{
		cout << returner[i] << " ";
	}

	return returner;
}

class TTreeNode : public TSortTable
{
	class Node
	{
	public:
		Node* left;
		Node* right;
		string data;	
		Node(string data = string(), Node* left = NULL, Node* right = NULL)
		{
			this->data = data;
			this->left = left;
			this->right = right;
			
		};
	};
	Node* Head;

	void rule(string person)
	{
		string true_person = string();
		for (int i = 0; i < person.length(); i++)
		{
			if (person[i] == '\t')
			{
				break;
			}
			true_person.push_back(person[i]);
		}
		//person = true_person;

		if (Head == NULL)
		{
			Head = new Node(person);
		}
		else
		{
			Node* current = Head;
			Node* prev = Head;
			queue<bool> path_prev;
			
			string data = string();


			while (current != NULL)
			{
				data = string();
				for (int i = 0; i < current->data.length(); i++)
				{
					if (current->data[i] == '\t')
					{
						break;
					}
					data.push_back(current->data[i]);
				}

				if (true_person >= data)
				{
					current = current->right;
					path_prev.push(true);
				}
				else
				{
					current = current->left;
					path_prev.push(false);
				}
			}

			while (path_prev.size() != 1)
			{
				if (path_prev.front())
				{
					path_prev.pop();
					prev = prev->right;
				}
				else
				{
					path_prev.pop();
					prev = prev->left;
				}
			}

			if (path_prev.front())
			{
				prev->right = new Node(person);
			}
			else
			{
				prev->left = new Node(person);
			}
		}
	}

public:
	TTreeNode(string path) : TSortTable(path)
	{
		/*string subjects = string();
		for (int i = 0; i < ammountTableColumns; i++)
		{
			subjects = subjects + tableMatrix[0][i];
		}
		subjects += '\n';*/

		vector<int> order = balanc(stringCount);
		cout << endl;

		while (!order.empty())
		{
			int number = order.front();
			order.erase(order.begin());
			string person = string();

			for (int i = 0; i < ammountTableColumns; i++)
			{
				person = person + tableMatrix[number][i];
				person += '\t';
			}

			rule(person);
		}
	}

	~TTreeNode()
	{
		Node* current = Head;
		Node* prev = Head;
		queue<bool> prev_path;
		while (Head->left != NULL)
		{
			current = Head;
			prev = Head;
			while (!prev_path.empty())
			{
				prev_path.pop();
			}

			while (current->left != NULL || current->right != NULL)
			{
				while (current->left != NULL)
				{
					current = current->left;
					prev_path.push(false);
				}
				while (current->right != NULL)
				{
					current = current->right;
					prev_path.push(true);
				}
			}
			delete current;
			

			while (prev_path.size() != 1)
			{
				if (prev_path.front())
				{
					prev_path.pop();
					prev = prev->right;
				}
				else
				{
					prev_path.pop();
					prev = prev->left;
				}
			}

			if (prev_path.front())
			{
				prev->right = NULL;
			}
			else
			{
				prev->left = NULL;
			}


		}

		while (Head->right != NULL)
		{
			current = Head;
			prev = Head;
			while (!prev_path.empty())
			{
				prev_path.pop();
			}

			while (current->left != NULL || current->right != NULL)
			{
				while (current->right != NULL)
				{
					current = current->right;
					prev_path.push(true);
				}

				while (current->left != NULL)
				{
					current = current->left;
					prev_path.push(false);
				}
			}
			delete current;
			

			while (prev_path.size() != 1)
			{
				if (prev_path.front())
				{
					prev_path.pop();
					prev = prev->right;
				}
				else
				{
					prev_path.pop();
					prev = prev->left;
				}
			}

			if (prev_path.front())
			{
				prev->right = NULL;
			}
			else
			{
				prev->left = NULL;
			}
		}

		current = Head;
		delete Head;
		Head = NULL;
		
	}

	string finder(string key) override
	{
		string data;

		string subject = string();
		for (int i = 0; i < ammountTableColumns; i++)
		{
			subject += tableMatrix[0][i];
			subject += '\t';
		}
		subject += '\n';

		if (key[0] >= '0' && key[0] <= '9')
		{
			string help = string();
			help.push_back(key[0]);
			int goNext = stoi(help);
			key.erase(0, 1);

			Node* current = Head;

			while (current != NULL)
			{
				data = string();
				for (int i = 0; i < current->data.length(); i++)
				{
					if (current->data[i] == '\t')
					{
						break;
					}
					data.push_back(current->data[i]);
				}

				if (key == data)
				{
					if (goNext != 0)
					{
						goNext--;
					}
					else
					{
						return subject + current->data;
					}
				}

				if (key > data)
				{
					current = current->right;
				}
				else
				{
					current = current->left;
				}
			}
			cout << "Нет такого ФИО (цифра)" << endl;
			return string();
		}
		else
		{
			Node* current = Head;

			while (current != NULL)
			{
				data = string();
				for (int i = 0; i < current->data.length(); i++)
				{
					if (current->data[i] == '\t')
					{
						break;
					}
					data.push_back(current->data[i]);
				}

				if (key == data)
				{
					
					return subject + current->data;
					
				}

				if (key > data)
				{
					current = current->right;
				}
				else
				{
					current = current->left;
				}
			}
			cout << "Нет такого ФИО" << endl;
			return string();
		}
		
	}

	void inserter(string person, string marksByUser) override
	{
		if (marksByUser.length() < (ammountTableColumns - 1) * 2)
		{
			cout << "мало оценок: < " << (ammountTableColumns - 1)<< endl;
			return void();
		}
		string marks = string();
		int i = 0;
		while (i < marksByUser.length() && i < (ammountTableColumns - 1) * 2)
		{
			marks.push_back(marksByUser[i]);
			if (marks[i] == ' ' )
			{
				marks[i] = '|';
			}
			i++;
		}
		marks.pop_back();
		string insert_sort = person + '|' + marks;
		cout << insert_sort << endl;
		TSortTable::inserter(insert_sort, "line");

		i = 0;
		marks = string();
		while (i < marksByUser.length() && i < (ammountTableColumns - 1) * 2)
		{
			if (marksByUser[i] == ' ')
			{
				marksByUser[i] = '\t';
			}
			marks.push_back(marksByUser[i]);
			i++;
		}
		person += '\t';
		person += marks;

		rule(person);
	}

	bool deleter(string key) override
	{
		TSortTable::deleter(key);

		Node* current = Head;
		Node* prev = Head;
		queue<bool> prev_path;
		if (key[0] >= '0' && key[0] <= '9')
		{
			string data = string();
			string help = string();
			help.push_back(key[0]);
			int goNext = stoi(help);
			key.erase(0, 1);

			while (current != NULL && current->data != key)
			{
				data = string();
				for (int i = 0; i < current->data.length(); i++)
				{
					if (current->data[i] == '\t')
					{
						break;
					}
					data.push_back(current->data[i]);
				}

				if (key == data)
				{
					if (goNext != 0)
					{
						goNext--;
					}
					else
					{
						break;
					}
				}


				if (key > data)
				{
					current = current->right;
					prev_path.push(true);
				}
				else
				{
					current = current->left;
					prev_path.push(false);
				}
			}

			if (current == NULL)
			{
				cout << "Нет такого ФИО (цифра)" << endl;
				return false;
			}

			if (current->left == NULL && current->right == NULL)
			{
				delete current;

				while (prev_path.size() != 1)
				{
					if (prev_path.front())
					{
						prev_path.pop();
						prev = prev->right;
					}
					else
					{
						prev_path.pop();
						prev = prev->left;
					}
				}

				if (prev_path.front())
				{
					prev->right = NULL;
				}
				else
				{
					prev->left = NULL;
				}

				return true;
			}
			else if (current->left == NULL || current->right == NULL)
			{

				while (prev_path.size() != 1)
				{
					if (prev_path.front())
					{
						prev_path.pop();
						prev = prev->right;
					}
					else
					{
						prev_path.pop();
						prev = prev->left;
					}
				}

				if (prev_path.front())
				{
					if (current->left != NULL)
					{
						prev->right = current->left;
						delete current;
					}
					else
					{
						prev->right = current->right;
						delete current;
					}
				}
				else
				{
					if (current->left != NULL)
					{
						prev->left = current->left;
						delete current;
					}
					else
					{
						prev->left = current->right;
						delete current;
					}
				}
				return true;
			}
			else
			{
				Node* swap = current;
				Node* swapPrev = current;
				swap = swap->right;
				if (swap->left != NULL)
				{
					swapPrev = swapPrev->right;
				}

				while (swap->left != NULL)
				{
					swap = swap->left;
					if (swapPrev->left != swap)
					{
						swapPrev = swapPrev->left;
					}
				}
				Node* swapLeft = swap->left;
				Node* swapRight = swap->right;

				Node* currentLeft = current->left;
				Node* currentRight = current->right;

				while (prev_path.size() != 1)
				{
					if (prev_path.front())
					{
						prev = prev->right;
						prev_path.pop();
					}
					else
					{
						prev = prev->left;
						prev_path.pop();
					}
				}

				if (prev_path.front())
				{
					prev->right = swap;
				}
				else
				{
					prev->left = swap;
				}

				swap->left = currentLeft;
				swap->right = current;


				if (swapPrev->left == swap)
				{
					swapPrev->left = current;
				}
				else
				{
					//swapPrev->right = current;
				}
				current->left = swapLeft;
				current->right = swapRight;


				/*while (!prev_path.empty())
				{
					prev_path.pop();
				}
				prev = Head;
				while (prev->data != current->data)
				{
					if (key > data)
					{
						current = current->right;
						prev_path.push(true);
					}
					else
					{
						current = current->left;
						prev_path.push(false);
					}
				}*/

				while (!prev_path.empty())
				{
					prev_path.pop();
				}

				prev = Head;

				while (prev->data != current->data)
				{
					if (prev->data > current->data)
					{

						prev = prev->left;
						prev_path.push(false);
					}
					else
					{
						prev = prev->right;
						prev_path.push(true);
					}
				}

				if (current->left == NULL && current->right == NULL)
				{
					delete current;

					prev = Head;
					while (prev_path.size() != 1)
					{
						if (prev_path.front())
						{
							prev_path.pop();
							prev = prev->right;
						}
						else
						{
							prev_path.pop();
							prev = prev->left;
						}
					}

					if (prev_path.front())
					{
						prev->right = NULL;
					}
					else
					{
						prev->left = NULL;
					}

					return true;
				}
				else if (current->left == NULL || current->right == NULL)
				{

					while (prev_path.size() != 1)
					{
						if (prev_path.front())
						{
							prev_path.pop();
							prev = prev->right;
						}
						else
						{
							prev_path.pop();
							prev = prev->left;
						}
					}

					if (prev_path.front())
					{
						if (current->left != NULL)
						{
							prev->right = current->left;
							delete current;
						}
						else
						{
							prev->right = current->right;
							delete current;
						}
					}
					else
					{
						if (current->left != NULL)
						{
							prev->left = current->left;
							delete current;
						}
						else
						{
							prev->left = current->right;
							delete current;
						}
					}
					return true;
				}


			}
			//else
			//{
			//	Node* swap = current;
			//	queue<bool> swap_path;
			//	Node* swap_prev = current;
			//	swap = swap->right;
			//	swap_path.push(true);
			//	while (swap->left != NULL)
			//	{
			//		swap = swap->left;
			//		swap_path.push(false);
			//	}
			//	
			//	while (swap_path.size() != 1)
			//	{
			//		if (swap_path.front())
			//		{
			//			swap_prev = swap_prev->right;
			//			swap_path.pop();
			//		}
			//		else
			//		{
			//			swap_prev = swap_prev->left;
			//			swap_path.pop();
			//		}
			//	}

			//	Node* helper;
			//	Node* helperleft = current->left;
			//	Node* helperright = current->right;

			//	current->right = swap->right;
			//	current->left = swap->left;


			//	if (swap_path.front())
			//	{
			//		swap_prev->right = current;
			//		swap_path.pop();
			//	}
			//	else
			//	{
			//		swap_prev->left = current;
			//		swap_path.pop();
			//	}

			//	

			//	swap->right = helperright;
			//	swap->left = helperleft;
			//	while (prev_path.size() != 1 && !prev_path.empty())
			//	{
			//		if (prev_path.front())
			//		{
			//			prev->right = current;
			//			prev_path.pop();
			//		}
			//		else
			//		{
			//			prev->left = current;
			//			prev_path.pop();
			//		}
			//	}

			//	if (prev_path.front())
			//	{
			//		prev->right = swap;
			//		prev_path.pop();
			//	}
			//	else
			//	{
			//		prev->left = swap;
			//		prev_path.pop();
			//	}

			//	
			//	//---
			//	while (!prev_path.empty())
			//	{
			//		prev_path.pop();
			//	}
			//	prev = Head;

			//	helper = Head;

			//	data = string();
			//	for (int i = 0; i < current->data.length(); i++)
			//	{
			//		if (current->data[i] == '\t')
			//		{
			//			break;
			//		}
			//		data.push_back(current->data[i]);
			//	}

			//	string true_person = string();

			//	while (helper->data != current->data)
			//	{
			//		true_person = string();
			//		for (int i = 0; i < helper->data.length(); i++)
			//		{
			//			if (helper->data[i] == '\t')
			//			{
			//				break;
			//			}
			//			true_person.push_back(helper->data[i]);
			//		}

			//		if (data >= true_person)
			//		{
			//			helper = helper->right;
			//			prev_path.push(true);
			//		}
			//		else
			//		{
			//			helper = helper->left;
			//			prev_path.push(false);
			//		}
			//	}



			//	//Node* a = Head;

			//	if (current->left == NULL && current->right == NULL)
			//	{
			//		delete current;

			//		while (prev_path.size() != 1)
			//		{
			//			if (prev_path.front())
			//			{
			//				prev_path.pop();
			//				prev = prev->right;
			//			}
			//			else
			//			{
			//				prev_path.pop();
			//				prev = prev->left;
			//			}
			//		}

			//		if (prev_path.front())
			//		{
			//			prev->right = NULL;
			//		}
			//		else
			//		{
			//			prev->left = NULL;
			//		}

			//		return true;
			//	}
			//	else if (current->left == NULL || current->right == NULL)
			//	{

			//		while (prev_path.size() != 1)
			//		{
			//			if (prev_path.front())
			//			{
			//				prev_path.pop();
			//				prev = prev->right;
			//			}
			//			else
			//			{
			//				prev_path.pop();
			//				prev = prev->left;
			//			}
			//		}

			//		if (prev_path.front())
			//		{
			//			if (current->left != NULL)
			//			{
			//				prev->right = current->left;
			//				delete current;
			//			}
			//			else
			//			{
			//				prev->right = current->right;
			//				delete current;
			//			}
			//		}
			//		else
			//		{
			//			if (current->left != NULL)
			//			{
			//				prev->left = current->left;
			//				delete current;
			//			}
			//			else
			//			{
			//				prev->left = current->right;
			//				delete current;
			//			}
			//		}
			//		return true;
			//	}

			//}
		}
		else
		{
			string data = string();
			while (current != NULL)
			{
				data = string();
				for (int i = 0; i < current->data.length(); i++)
				{
					if (current->data[i] == '\t')
					{
						break;
					}
					data.push_back(current->data[i]);
				}

				if (key == data)
				{	
					break;
				}

				if (key > data)
				{
					current = current->right;
					prev_path.push(true);
				}
				else
				{
					current = current->left;
					prev_path.push(false);
				}
			}

			if (current == NULL)
			{
				cout << "Нет такого ФИО (цифра)" << endl;
				return false;
			}

			if (current->left == NULL && current->right == NULL)
			{
				delete current;

				while (prev_path.size() != 1)
				{
					if (prev_path.front())
					{
						prev_path.pop();
						prev = prev->right;
					}
					else
					{
						prev_path.pop();
						prev = prev->left;
					}
				}

				if (prev_path.front())
				{
					prev->right = NULL;
				}
				else
				{
					prev->left = NULL;
				}

				return true;
			}
			else if (current->left == NULL || current->right == NULL)
			{

				while (prev_path.size() != 1)
				{
					if (prev_path.front())
					{
						prev_path.pop();
						prev = prev->right;
					}
					else
					{
						prev_path.pop();
						prev = prev->left;
					}
				}

				if (prev_path.front())
				{
					if (current->left != NULL)
					{
						prev->right = current->left;
						delete current;
					}
					else
					{
						prev->right = current->right;
						delete current;
					}
				}
				else
				{
					if (current->left != NULL)
					{
						prev->left = current->left;
						delete current;
					}
					else
					{
						prev->left = current->right;
						delete current;
					}
				}
				return true;
			}
			else
			{
				Node* swap = current;
				Node* swapPrev = current;
				swap = swap->right;
				if (swap->left != NULL)
				{
					swapPrev = swapPrev->right;
				}

				while (swap->left != NULL)
				{
					swap = swap->left;
					if (swapPrev->left != swap)
					{
						swapPrev = swapPrev->left;
					}
				}
				Node* swapLeft = swap->left;
				Node* swapRight = swap->right;

				Node* currentLeft = current->left;
				Node* currentRight = current->right;

				while (prev_path.size() != 1)
				{
					if (prev_path.front())
					{
						prev = prev->right;
						prev_path.pop();
					}
					else
					{
						prev = prev->left;
						prev_path.pop();
					}
				}

				if (prev_path.front())
				{
					prev->right = swap;
				}
				else
				{
					prev->left = swap;
				}

				swap->left = currentLeft;
				swap->right = current;


				if (swapPrev->left == swap)
				{
					swapPrev->left = current;
				}
				else
				{
					//swapPrev->right = current;
				}
				current->left = swapLeft;
				current->right = swapRight;

				while (!prev_path.empty())
				{
					prev_path.pop();
				}

				prev = Head;

				while (prev->data != current->data)
				{
					if (prev->data > current->data)
					{

						prev = prev->left;
						prev_path.push(false);
					}
					else
					{
						prev = prev->right;
						prev_path.push(true);
					}
				}

				if (current->left == NULL && current->right == NULL)
				{
					delete current;

					prev = Head;
					while (prev_path.size() != 1)
					{
						if (prev_path.front())
						{
							prev_path.pop();
							prev = prev->right;
						}
						else
						{
							prev_path.pop();
							prev = prev->left;
						}
					}

					if (prev_path.front())
					{
						prev->right = NULL;
					}
					else
					{
						prev->left = NULL;
					}

					return true;
				}
				else if (current->left == NULL || current->right == NULL)
				{

					while (prev_path.size() != 1)
					{
						if (prev_path.front())
						{
							prev_path.pop();
							prev = prev->right;
						}
						else
						{
							prev_path.pop();
							prev = prev->left;
						}
					}

					if (prev_path.front())
					{
						if (current->left != NULL)
						{
							prev->right = current->left;
							delete current;
						}
						else
						{
							prev->right = current->right;
							delete current;
						}
					}
					else
					{
						if (current->left != NULL)
						{
							prev->left = current->left;
							delete current;
						}
						else
						{
							prev->left = current->right;
							delete current;
						}
					}
					return true;
				}


			}

			return bool();
		}
	}

	void show()
	{
		int size = 0;

		Node* current = Head;
		Node* prev = Head;
		queue<bool> prev_path;
		while (Head->left != NULL)
		{
			current = Head;
			prev = Head;
			while (!prev_path.empty())
			{
				prev_path.pop();
			}

			while (current->left != NULL || current->right != NULL)
			{
				while (current->left != NULL)
				{
					current = current->left;
					prev_path.push(false);
				}
				while (current->right != NULL)
				{
					current = current->right;
					prev_path.push(true);
				}
			}
			size++;
			cout << current->data << endl;
			delete current;


			while (prev_path.size() != 1)
			{
				if (prev_path.front())
				{
					prev_path.pop();
					prev = prev->right;
				}
				else
				{
					prev_path.pop();
					prev = prev->left;
				}
			}

			if (prev_path.front())
			{
				prev->right = NULL;
			}
			else
			{
				prev->left = NULL;
			}


		}

		cout << endl << endl;

		while (Head->right != NULL)
		{
			current = Head;
			prev = Head;
			while (!prev_path.empty())
			{
				prev_path.pop();
			}

			while (current->left != NULL || current->right != NULL)
			{
				while (current->right != NULL)
				{
					current = current->right;
					prev_path.push(true);
				}

				while (current->left != NULL)
				{
					current = current->left;
					prev_path.push(false);
				}
			}
			size++;
			cout << current->data << endl;
			delete current;


			while (prev_path.size() != 1)
			{
				if (prev_path.front())
				{
					prev_path.pop();
					prev = prev->right;
				}
				else
				{
					prev_path.pop();
					prev = prev->left;
				}
			}

			if (prev_path.front())
			{
				prev->right = NULL;
			}
			else
			{
				prev->left = NULL;
			}
		}

		cout << endl << endl;


		current = Head;
		size++;
		cout << Head->data << endl;
		delete Head;
		Head = NULL;
		//---
		size++;
		vector<int> order = balanc(size);
		cout << endl;

		while (!order.empty())
		{
			int number = order.front();
			order.erase(order.begin());
			string person = string();

			for (int i = 0; i < ammountTableColumns; i++)
			{
				person = person + tableMatrix[number][i];
				person += '\t';
			}

			rule(person);
		}

	}
};


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
	//int SIZE = 33 * 33 * 33;
	int hash;
	hash = (fullName[0] - 'А')%256 + (fullName[1] - 'А') % 256 *32  + (fullName[2] - 'А') % 256 * 32 * 32;
	return hash;// % SIZE;
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
				//cout << *next(hashTable[index].begin(), index_in_list) << endl;
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
			cout << "Ошибка: неправильное кол-во оценок. Кол-во = "<<number_of_marks << endl;
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
		TScanTable::inserter(person+ marks_to_scanTable, "line");
	}
};


int main()
{
	setlocale(LC_ALL, "rus");

	

	string path = "oldTable.txt";


	THashTable table4(path);
	table4.inserter("Антон А.И.", "4 4 4");
	table4.finder("Антон А.И.");
	//table4.finder("1Захаров А.А.");
	//table4.deleter("1Захаров А.А.");
	//table4.finder("0Захаров А.А.");
	
	/*list<int> a;
	a.push_back(1);
	a.push_back(2);
	a.erase(next(a.begin(), 1));*/


	/*string str = "Захар А.А.";
	if (str.find("Gахаов А.А.") != string::npos)
	{
		cout << "yes" << endl;
	}*/
	//table4.printArray();

	/*list<string> b;
	if (*b.begin() == string())
	{
		cout << "yes" << endl;
	}*/
	//b.push_back("aaa");
	//b.push_back("bbb");
	//b.push_back("ccc");
	//cout << *next(b.begin(), 2);

	/*vector<list<string>> a = 3;
	if (*a[0].begin() == "")
	{
		cout << "yes";
	}
	cout << *a[0].begin();*/

	/*vector<int> a;
	a.reserve(3);
	cout << a.capacity() << endl;
	for (int i = 0; i < a.capacity(); i++)
	{
		a.push_back(NULL);
	}
	if (a[2] == NULL)
	{
		cout << "yes";
		a[2] = 3;
	}
	if (a[2] == NULL)
	{
		cout << "yes";
		a[2] = NULL;
	}*/

	//cout<<"res " << a[0] << " " << a[1];

	//cout << "H " << Hash("ААА")<< endl;
	//cout << "H " << Hash("ЯЯЯ") << endl;
	//cout << toupper('а') - 'А';
	//TTreeNode table3(path);

	////table3.inserter("alex", "1 2 3 ");
	//cout<<table3.finder("Ааранов А.А.")<<endl;
	//table3.deleter("Ааранов А.А.");
	//cout << table3.finder("Ааранов А.А.") << endl;

	//cout << endl << endl;
	//table3.show();


	//cout<< table3.finder("Баранов А.А.") <<endl;

	//TSortTable table2(path);

	////table2.printMatrix();

	//table2.deleter("Баранов А.А.");
	//table2.deleter("*Русский язык");

	//table2.deleter("Баранов А.А.");
	//table2.deleter("Русский язы");

	//table2.finder("*Математика");
	//table2.finder("Наумов Н.В.");

	//table2.finder("2");

	//table2.inserter("матеша|1|2|3|4|5|6|7|8|9","column");

	//table2.printMatrix();


	
	
	return 0;
	
}
