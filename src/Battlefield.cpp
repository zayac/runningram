/* 
 * File:   Battlefield.cpp
 * Author: necto
 * 
 * Created on January 8, 2010, 12:30 AM
 */

#include <fstream>
#include "Battlefield.h"

using std::ifstream;

#define CELL(x, y) cells[size.x*(y) + x]

Battlefield::Battlefield ():cells (0)
{
}
//--------------------------------------------------------------------------------------------------
Battlefield::~Battlefield ()
{
	if (cells) delete [] cells;
}
//--------------------------------------------------------------------------------------------------
bool Battlefield::Load_from_file (char* fname)
{
	ifstream file (fname);
	if (file.bad()) return false;

	file >>size.x;
	file >>size.y;

	cells = new char[size.x*size.y];
	if (cells == 0) return false;
	Clean_field ('0');

	for (int i = 0; i < size.y*size.x; )
	{
		char symb = file.get();
		if (symb == '\n') continue;
		cells[i++] = symb;
		
		if (file.eof()) return false;
	}

	file.close();
	return true;
}
//--------------------------------------------------------------------------------------------------
void Battlefield::Clean_field (char fill_cell)
{
	for (int i = size.x*size.y - 1; i >= 0; --i)
	{
		cells[i] = fill_cell;
	}
}
//--------------------------------------------------------------------------------------------------

#undef CELL