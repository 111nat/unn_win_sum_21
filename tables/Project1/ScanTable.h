#pragma once
#include "AbstractTable.h"


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
