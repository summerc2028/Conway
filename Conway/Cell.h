#include <stdlib.h>

#ifndef Cell_h
#define Cell_h

using namespace std;

class Cell
{
	public:
		int _x;
		int _y;
		Cell(int, int);
		int hashCode();
		bool operator==(const Cell& c) const;
	private:
		int hashing;
};

Cell::Cell(int x, int y)
{
	_x = x;
	_y = y;
	hashing = x*11+y*17;
}

int Cell::hashCode()
{
	return hashing*37;
}

bool Cell::operator==(const Cell& c) const 
{
	return (_x== c._x && _y == c._y);
}
#endif