#pragma once


#include <iostream>
using namespace std;

class Coordinates
{

public:
	Coordinates(int top, int left, int bottom, int right);

	bool operator == (Coordinates const & coor);

	int top();
	int left();
	int bottom();
	int right();

	friend ostream & operator<<( std::ostream & out, Coordinates const & coor);

private:
	
	int m_top;
	int m_left;
	int m_bottom;
	int m_right;
};
