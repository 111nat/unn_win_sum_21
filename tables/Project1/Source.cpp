#include <iostream>
#include <fstream>
#include <Windows.h>
#include <string>
#include <stack>
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

int main()
{
	setlocale(LC_ALL, "rus");
	string path = "oldTable.txt";

	TSortTable table2(path);

	//table2.printMatrix();

	table2.deleter("Баранов А.А.");
	table2.deleter("*Русский язык");

	table2.deleter("Баранов А.А.");
	table2.deleter("Русский язы");

	table2.finder("*Математика");
	table2.finder("Наумов Н.В.");

	table2.finder("2");

	table2.inserter("яреяре|1|2|3|4|5|6|7|8|9","column");

	table2.printMatrix();

	//TScanTable table1(path);


	//table1.deleter("2");
	//table1.deleter("Математика");

	//table1.inserter("сергей ф.ф.|5|4|3|2", "line");
	//table1.inserter("arteml a.q.|1|2|3|4", "line");

	//table1.inserter("proga|1|2|3|4", "column");
	//table1.inserter("matan|1|2|3|4", "column");
	//
	//table1.printMatrix();

	//cout << endl << endl;

	//table1.finder("matan");
	//cout << endl;

	//table1.finder("ФИО");
	//cout << endl;
	//table1.finder("Математика");//net
	//cout << endl;

	//table1.finder("сергей ф.ф.");
	//cout << endl;

	//table1.finder("2");
	//cout << endl;


	return 0;
	
}
