#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <limits>
#include <string>
#include <list>
#include "HashTable.h"

using namespace std;

struct Cell
{
	int _x;
	int _y;
	bool isAlive;

	bool operator==(const Cell& c) const {return _x == c._x && _y == c._y;}
	int hashCode() const {return (_x*31+_y*37);}
};

int XMAX = 0;
int YMAX = 0;

bool alive(HashTable<Cell>* table, Cell me)
{
	// retrieve cell me from the table
	if (table->retrieve(me)) return me.isAlive;
	else return false;
}

int countNeighbors(HashTable<Cell>* table, int x, int y)
{
	int count = 0;
	Cell tempCell;
	
	// count the 6 cells above and below current location
	for (int i = -1; i <= 1; i++)
	{
		tempCell._x = x + i;
		tempCell._y = y - 1;
		if (alive(table, tempCell)) count++;
		tempCell._y = y + 1;
		if (alive(table, tempCell)) count++;
	}
	// count left and right cells
	tempCell._x = x - 1;
	tempCell._y = y;
	if (alive(table, tempCell)) count++;
	tempCell._x = x + 1;
	if (alive(table, tempCell)) count++;
	return count;
}

void nextTable(HashTable<Cell>* cur, HashTable<Cell>* next)
{
	Cell tempCell;
	tempCell._x = 1;
	tempCell._y = 1;
	next->clear();
	while (tempCell._y <= YMAX)
	{
		while (tempCell._x <= XMAX)
		{
			if (cur->retrieve(tempCell))
			{
				int aliveNeighbor = countNeighbors(cur, tempCell._x, tempCell._y);
				if (aliveNeighbor < 2 || aliveNeighbor > 3) tempCell.isAlive = false;
				else if (aliveNeighbor == 3) tempCell.isAlive = true;
				next->insert(tempCell);
			}
			tempCell._x++;
		}
		tempCell._x = 1;
		tempCell._y++;
	}
}

void display80By20Table(HashTable<Cell>* table, Cell startCell)
{
	int x = startCell._x;
	int y = startCell._y;
	int xBoundary = x + 79;
	int yBoundary = y + 20;
	Cell tempCell;
	while (y <= YMAX && y <= yBoundary)
	{
		tempCell._y = y;
		while (x <= XMAX && x <= xBoundary)
		{
			tempCell._x = x;
			if (table->retrieve(tempCell) && tempCell.isAlive)
				cout << '*';
			else
				cout << ' ';
			x++;
		}
		x = 1;
		y++;
		cout << endl;
	}
}

int nextTotalAliveCellsRight(HashTable<Cell>* table, Cell startCell, int pretotal)
{
	int yBoundary = startCell._y + 19;
	int firstColumnTotal = 0;
	int nextColumnTotal = 0;
	Cell temp;
	Cell temp2;
	temp._x = startCell._x;
	temp2._x = startCell._x + 80;
	for (int i = startCell._y; i <= yBoundary; i++)
	{
		temp._y = i;
		temp2._y = i;
		if (alive(table, temp)) firstColumnTotal++;
		if (alive(table, temp2)) nextColumnTotal++;
	}
	return (pretotal - firstColumnTotal + nextColumnTotal);
}

int nextTotalAliveCellsDown(HashTable<Cell>* table, Cell startCell, int pretotal)
{
	int xBoundary = startCell._x + 79;
	int firstRowTotal = 0;
	int nextRowTotal = 0;
	Cell temp;
	Cell temp2;
	temp._y = startCell._y;
	temp2._y = startCell._y + 20;
	for (int i = startCell._x; i <= xBoundary; i++)
	{
		temp._x = i;
		temp2._x = i;
		if (alive(table, temp)) firstRowTotal++;
		if (alive(table, temp2)) nextRowTotal++;
	}
	return (pretotal - firstRowTotal + nextRowTotal);
}

int totalAliveCells(HashTable<Cell>* table, Cell startCell)
{
	int xBoundary = startCell._x + 79;
	int yBoundary = startCell._y + 19;
	int aliveTotal = 0;
	Cell temp;
	for (int i = 1; i <= yBoundary; i++)
	{
		temp._y = i;
		for (int j = 1; j <= xBoundary; j++)
		{
			temp._x = j;
			if (alive(table, temp)) aliveTotal++;
		}
	}
	return aliveTotal;
}

Cell findMostAliveCells(HashTable<Cell>* table)
{
	Cell maxCell;
	maxCell._x = 1;
	maxCell._y = 1;
	Cell temp;
	temp._x = 1;
	temp._y = 1;
	int maxTotal;
	int currentTotal;
	int startTotal;

	currentTotal = totalAliveCells(table, temp);
	maxTotal = currentTotal;
	startTotal = currentTotal;
	while (temp._y + 19 <= YMAX)
	{
		while (temp._x + 79 < XMAX)
		{
			temp._x++;
			currentTotal = nextTotalAliveCellsRight(table, temp, currentTotal);
			if (currentTotal > maxTotal)
			{
				maxTotal = currentTotal;
				maxCell._x = temp._x;
				maxCell._y = temp._y;
			}
		}
		temp._x = 1;
		temp._y++;
		currentTotal = nextTotalAliveCellsDown(table, temp, startTotal);
		startTotal = currentTotal;
		if (currentTotal > maxTotal)
		{
				maxTotal = currentTotal;
				maxCell._x = temp._x;
				maxCell._y = temp._y;
		}
	}
	return maxCell;
}

int display(HashTable<Cell>* table)
{
	if (XMAX > 80 || YMAX > 20)
	{
		Cell startCell = findMostAliveCells(table);
		display80By20Table(table, startCell);
	}
	else
	{
		Cell startCell;
		startCell._x = 1;
		startCell._y = 1;
		display80By20Table(table, startCell);
		return 0;
	}
}

void initialize(HashTable<Cell>* cur, FILE* file)
{
	XMAX = 0;
	YMAX = 0;

	Cell tempCell;
	tempCell._x = 1;
	tempCell._y = 1;

	char c = fgetc(file);
	while (c != EOF)
	{
		if (c == '*' || c == ' ')
		{
			if (tempCell._x == 1) YMAX++;
			if (c == '*') tempCell.isAlive = true;
			else tempCell.isAlive = false;
			cur->insert(tempCell);
			tempCell._x++;
		}
		else if (c == '\n')
		{
			if (tempCell._x - 1 > XMAX) XMAX = tempCell._x - 1;
			tempCell._y++;
			tempCell._x = 1;
		}
		else
		{
			cout << "There is an error in the input file" << endl;
			return;
		}
		c = fgetc(file);
	}
}

int main()
{
	HashTable<Cell> table;
	HashTable<Cell> table2;
	string temp;

	char fileName[100];
	GET_FILE_NAME:
	cout << "Please enter the input file name: ";
	cin >> fileName;
	FILE* inFile = fopen(fileName,"r");
	cin.ignore(numeric_limits <streamsize> ::max(), '\n' );
	if (inFile == NULL) 
	{
		perror("Error occurred");
		goto GET_FILE_NAME;
	}
	initialize(&table, inFile);
	fclose(inFile);
	cout << "The initial Conway world:" << endl;
	display(&table);

	char c;
	GET_COMMAND:
	cout << "Please enter your command (less than 100 characters): " << endl
		<< "(ENTER: Next generation" << endl
		<< " Integer: The next N th generation" << endl
		<< " Q or q: Quit): ";
	c = getchar();
	// User pressed ENTER
	if (c == '\n')
	{
		nextTable(&table, &table2);
		table = table2;
		display(&table);
		goto GET_COMMAND;
	}
	// User entered number
	else if (isdigit(c))
	{
		char command[100];
		int i = 0;
		while (c != '\n')
		{
			if (isdigit(c))
			{
				command[i] = c;
				i++;
				c = getchar();		
			}
			else goto WRONG_COMMAND;
		}
		int generation = atoi(command);
		while (generation > 0)
		{
			nextTable(&table, &table2);
			table = table2;
			generation--;
		}
		display(&table);
		goto GET_COMMAND;
	}
	// User entered q or Q
	else if (c == 'q' || c == 'Q')
	{
		c = getchar();
		if (c == '\n') goto EXIT;
		else goto WRONG_COMMAND;
	}

	WRONG_COMMAND:
	getline(cin, temp);
	cout << "There is an error in the command." << endl;
	goto GET_COMMAND;

	EXIT:
	return 0;
}