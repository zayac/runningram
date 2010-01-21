/* 
 * File:   Activeman.cpp
 * Author: necto
 * 
 * Created on January 12, 2010, 12:32 AM
 */

#include "Activeman.h"
#include "Car.h"
#include "Battlefield.h"
//
//Activeman::Activeman () { }
//
//Activeman::Activeman (const Activeman& orig) { }
//
//Activeman::~Activeman () { }

//--------------------------------------------------------------------------------------------------
void Activeman::Activate (float dt)
{
	assert(Ok());
	for (iterator i = begin(); i != end(); ++i)
		(**i).Actions (dt);
}
//--------------------------------------------------------------------------------------------------
void Activeman::Draw (Canvas* c)
{
	assert(Ok());
	for (iterator i = begin(); i != end(); ++i)
		(**i).Draw (c);
}
//--------------------------------------------------------------------------------------------------
void Activeman::Collis_brd (const Battlefield* bf)
{
	assert(Ok());
	assert (bf != 0 && bf->Ok());
#define CELL(a, b) cells[size.x*(b) + a]

	const char* cells = bf->Get_cells ();
	Point size = bf->Get_size ();
	int csize = bf->Get_cell_size();
	Point full_size = csize*size;
	for (iterator i = begin(); i != end(); ++i)
	{
		float r = (**i).Get_r();
		Point pos = (**i).Get_pos ().To<int>();
		Point lup = pos - Point (r, r);
		Point rdown = pos + Point (r, r);

		if (lup.x > full_size.x) continue;
		if (lup.y > full_size.y) continue;
		if (rdown.x < 0) continue;
		if (rdown.y < 0) continue;

		if (lup.x < 0) lup.x = 0;
		if (lup.y < 0) lup.y = 0;
		if (rdown.y > full_size.y) rdown.y = full_size.y;
		if (rdown.x > full_size.x) rdown.x = full_size.x;

		for (int x = 0; x < size.x; ++x)
			for (int y = 0; y < size.y; ++y)
				if (CELL(x, y) != '1')
				{
					if (x == 6 && y == 0)//!!!deprecated
					{
						int a = 3;
						a++;
						a-= 3;
					}
					Rect cell(x*csize, y*csize, csize, csize);//Can to check condition p(cell, i) < i->r
					(**i).Collis_brd (cell);
				}
	}
#undef CELL
}
//--------------------------------------------------------------------------------------------------
bool Activeman::Ok() const
{
	for (const_iterator i = begin(); i != end(); ++i)
		if (*i == 0 || !(**i).Ok()) return false;
	return true;
}
//--------------------------------------------------------------------------------------------------