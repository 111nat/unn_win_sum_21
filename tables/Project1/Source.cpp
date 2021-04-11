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
private:
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
				//----------
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
				//cout << tableMatrix[0][ammountTableColumns - 1] << endl;
			}
			//cout << tableMatrix[0][ammountTableColumns - 1] << endl;

		}

		if (key == "line")
		{

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
			for (int i = 0; i < stringCount; i++)
			{
				if (key == "deleted")
				{
					cout << "Слово не найдено" << endl;
					return string();
				}

				if (key == tableMatrix[i][0])
				{
					string returner = string();
					for (int j = 0; j < ammountTableColumns; j++)
					{
						returner = returner + tableMatrix[0][j];
						returner += " ";
					}
					returner += "\n";
					for (int j = 0; j < ammountTableColumns; j++)
					{
						returner = returner + tableMatrix[i][j];
						if (j + 1 == stringCount)
						{
							break;
						}
						//returner = returner + " ";
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
						if (i != 0)
						{
							returner = returner + tableMatrix[j][0];
							returner += "\t";
						}
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


int main()
{
	setlocale(LC_ALL, "rus");
	string path = "oldTable.txt";
	TScanTable table1(path);
	//table1.finder("Баранов А.А.");
	//table1.finder("Математика");
	//table1.finder("ФИО");
	//table1.finder("20");
	//table1.print();
	//table1.inserter("hello", "1321");
	
	table1.deleter("2");
	table1.deleter("Математика");

	table1.inserter("Прога|5|4|3|2", "column");
	table1.inserter("Физра|1|2|3|4", "column");
	

	table1.printMatrix();

	return 0;
	
}
