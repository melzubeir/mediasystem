#include "Coordinates.h"


Coordinates::Coordinates(int top, int left, int bottom, int right)
{
	m_top	= top;
	m_left	= left;
	m_bottom= bottom;
	m_right = right;
}

bool Coordinates::operator == (Coordinates const & coor)
{
	return ( 
		(m_top		==	coor.m_top)		&&
		(m_left		==	coor.m_left)	&&
		(m_bottom	==	coor.m_bottom)	&&
		(m_right	==	coor.m_right) 
		);
}


int Coordinates::top()
{
	return m_top;
}

int Coordinates::left()
{
	return m_left;
}

int Coordinates::bottom()
{
	return m_bottom;
}

int Coordinates::right()
{
	return m_right;
}


ostream & operator<<( std::ostream & out, Coordinates const & coor)
{
	out << "(" << coor.m_top << "," << coor.m_left << ")<->(" << coor.m_bottom << "," << coor.m_right << ")" << endl;
	return out;
}


