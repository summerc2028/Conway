#include <stdlib.h>
#include <list>
#include <algorithm>

#ifndef HashTable_h
#define HashTable_h

using namespace std;

template <class DataType>
class HashTable
{
	public:
    HashTable();
    bool insert(DataType &item);
    bool retrieve(DataType &item);
    bool remove(DataType &item);
	bool removeAny(DataType &item);
    void clear();
    int size() const;

	private:
    list<DataType> table[331];
	int numberOfElements;
	int tableSize;
};

template <class DataType>
HashTable<DataType>::HashTable()
{
  list<DataType> table[331];
  numberOfElements = 0;
  tableSize = 331;
}

template <class DataType>
bool HashTable<DataType>::insert(DataType &item)
{
	bool insertSucceed = false;
	// get the index of the linked list to insert.
	int index = item.hashCode() % tableSize;
	if (index < 0) index += tableSize;

	// insert into the linked list
	list<DataType>::iterator iter = find(table[index].begin(), table[index].end(), item);
	if (iter == table[index].end())
	{
		table[index].push_back(item);
		numberOfElements++;
		insertSucceed = true;
	}
	return insertSucceed;
}

template <class DataType>
bool HashTable<DataType>::retrieve(DataType &item)
{
	bool hasItem = false;
	int index = item.hashCode() % tableSize;
	if (index < 0) index += tableSize;

	// find the item in a linked list according to the hash code
	list<DataType>::iterator iter = find(table[index].begin(), table[index].end(), item);
	if (iter != table[index].end()) 
	{
		item = *iter;
		hasItem = true;
	}
	return hasItem;	
}

template <class DataType>
bool HashTable<DataType>::remove(DataType &item)
{
	bool removeSucceed = false;
	int index = item.hashCode() % tableSize;
	if (index < 0) index += tableSize;

	// find the item in a linked list according to the hash code
	list<DataType>::iterator iter = find(table[index].begin(), table[index].end(), item);
	if (iter != table[index].end()) 
	{
		item = *iter;
		table[index].erase(iter);
		removeSucceed = true;
		numberOfElements--;
	}
	return removeSucceed;
}

template <class DataType>
bool HashTable<DataType>::removeAny(DataType &item)
{
	bool removeSucceed = false;
	int i = 0;
	// find the first nonempty list in the table and remove the first element
	while (!removeSucceed && i < tableSize)
	{
		if (!table[i].empty())
		{
			item = table[i].front();
			table[i].pop_front();
			removeSucceed = true;
			numberOfElements--;
		}
		i++;
	}
	return removeSucceed;
}

template <class DataType>
void HashTable<DataType>::clear()
{
	for (int i = 0; i < tableSize; i++)
	{
		table[i].clear();
	}
	numberOfElements = 0;
}

template <class DataType>
int HashTable<DataType>::size() const
{
	return numberOfElements;
}

#endif