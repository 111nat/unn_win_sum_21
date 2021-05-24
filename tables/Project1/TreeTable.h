#pragma once
#include "ScanTable.h"

class TTreeTable : public TScanTable
{
	struct node
	{
		string key = string(), person;
		int height;
		node* left;
		node* right;
		node(string k)
		{
			for (int i = 0; i < k.length(); i++)
			{
				if (k[i] == '|') break;
				key.push_back(k[i]);
			}

			person = k;
			left = right = 0;
			height = 1;
		}
	};

	int height(node* p)
	{
		return p ? p->height : 0;
	}

	int bfactor(node* p)
	{
		return height(p->right) - height(p->left);
	}

	void fixheight(node* p)
	{
		int hl = height(p->left);
		int hr = height(p->right);
		p->height = (hl > hr ? hl : hr) + 1;
	}

	node* rotateRight(node* p)
	{
		node* q = p->left;
		p->left = q->right;
		q->right = p;
		if (head == p)
		{
			head = q;
		}
		fixheight(p);
		fixheight(q);
		return q;
	}

	node* rotateLeft(node* q)
	{
		node* p = q->right;
		q->right = p->left;
		p->left = q;
		if (head == q)
		{
			head = p;
		}
		fixheight(q);
		fixheight(p);
		return p;
	}

	node* balance(node* p)
	{
		fixheight(p);
		if (bfactor(p) == 2)
		{
			if (bfactor(p->right) < 0)
				p->right = rotateRight(p->right);
			return rotateLeft(p);
		}
		if (bfactor(p) == -2)
		{
			if (bfactor(p->left) > 0)
				p->left = rotateLeft(p->left);
			return rotateRight(p);
		}
		return p;
	}

	node* insert(node* p, string k)
	{
		string fullname = string();
		for (int i = 0; i < k.length(); i++)
		{
			if (k[i] == '|') break;
			fullname.push_back(k[i]);
		}

		if (!p)
		{
			return new node(k);
		}
		if (fullname < p->key)
			p->left = insert(p->left, k);
		else
			p->right = insert(p->right, k);
		return balance(p);
	}

	void swaper(node* first, node* second)
	{
		swap(first->key, second->key);
		swap(first->person, second->person);
	}

	bool del(node* p, string key)
	{
		if (!p)
		{
			cout << "нет такого ФИО" << endl;
			return false;
		}

		if (key == p->key)
		{
			node* current = p;
			if (p->left != NULL && p->right != NULL)
			{
				current = current->right;

				while (current->left != NULL)
				{
					current = current->left;
				}

				swaper(p, current);
				p = current;
			}

			if (p->right != NULL || p->left != NULL)
			{
				if (p->right != NULL)
				{
					swaper(p, p->right);
					p = p->right;
				}
				else
				{
					swaper(p, p->left);
					p = p->left;
				}
			}

			p = NULL;
			return true;
		}

		if (key < p->key)
		{
			del(p->left, key);
		}
		else
		{
			del(p->right, key);
		}
		balance(p);
		return true;
	}

	node* head;
	int ammountMarks;
	string subjects;
public:
	TTreeTable(string path) :TScanTable(path)
	{
		subjects = array[0];

		head = insert(head, array[1] + subjects);
		for (int i = 2; i < stringCount; i++)
		{
			insert(head, array[i] + subjects);
		}
	}

	string finder(string key) override
	{
		node* current = head;

		while (current != NULL)
		{
			if (key == current->key)
			{
				return current->person;
			}

			if (key > current->key)
			{
				current = current->right;
			}
			else
			{
				current = current->left;
			}


		}
		cout << "нет такого человека" << endl;
		return string();
	}

	void inserter(string key, string marks) override
	{
		while (marks.length() != ammountMarks)
		{
			if (marks.length() < ammountMarks)
			{
				marks += '0';
			}
			else
			{
				string marks_helper = string();
				for (int i = 0; i < ammountMarks; i++)
				{
					marks_helper.push_back(marks[i]);
				}
				marks = marks_helper;
			}
		}

		for (int i = 1; i < marks.length(); i++)
		{
			marks.insert(marks.begin() + i, '|');
			i++;
			continue;
		}

		insert(head, key + marks);
	}

	bool deleter(string key) override
	{

		return del(head, key);
	}

	~TTreeTable()
	{
		node* current = head;
		node* prev = head;
		queue<bool> prev_path;
		while (head->left != NULL)
		{
			current = head;
			prev = head;
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

		while (head->right != NULL)
		{
			current = head;
			prev = head;
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

		current = head;
		delete head;
		head = NULL;

	}
};
