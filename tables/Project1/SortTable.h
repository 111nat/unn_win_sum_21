#pragma once
#include "ScanTable.h"

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

		int mid = (max - min) / 2 + min;
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
			for (int i = 0; i < key.length(); i++)
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
