#pragma once
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <string>
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
			cout << "������ �������� �����" << endl;
		}
		else
		{
			cout << "���� ������ ��� ������" << endl;
			while (file.get(textHelper))
			{
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
			cout << "������ �������� �����" << endl;
		}
		else
		{
			cout << "���� ������ ��� ������" << endl;
			file << newTable;

		}
		file.close();
		return string();
	}
	else
	{
		cout << "������ ������� fileWorker" << endl;
		return string();
	}
}