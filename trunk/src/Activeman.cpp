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
Canvas* globb = 0;//!!! deprecated, must be deleted
void Activeman::Draw (Canvas* c)
{
	globb = c;

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
					int left = x*csize;
					int right = left + csize;//cell borders
					int up = y*csize;
					int down = up + csize;

					if (right < lup.x) continue;
					if (left > rdown.x) continue;//necessary conditions
					if (up > rdown.y) continue;
					if (down < lup.y) continue;

					if (pos.y < up || pos.y > down)//we need a horisontal full lenght wall
					{
						int nx = x;

						while (++nx < size.x && right <= rdown.x)
						{
							if (CELL(nx, y) != '1'||
								(pos.y < up && y - 1 >=  0 && CELL(nx, y-1) != '1') ||	//for gnawed corners
								(pos.y > down && y + 1 < size.y && CELL(nx, y+1) != '1'))
								right += csize;			//cell alliance expansion
							else break;
						}
						nx = x;
						while (--nx >= 0 && left > lup.x)
						{
							if (CELL(nx, y) != '1'||
								(pos.y < up && y - 1 >=  0 && CELL(nx, y-1) != '1') ||	//for gnawed corners
								(pos.y > down && y + 1 < size.y && CELL(nx, y+1) != '1'))
								left -= csize;			//cell alliance expansion
							else break;
						}
					}
					else // we need a vertical full lenght wall
					{
						int ny = y;
						while (++ny < size.y && down <= rdown.y)
						{
							if (CELL(x, ny) != '1'||
								(pos.x < left && x - 1 >=  0 && CELL(x-1, ny) != '1') ||	//for gnawed corners
								(pos.x > right && x + 1 < size.x && CELL(x+1, ny) != '1'))
								down += csize;			//cell alliance expansion
							else break;
						}
						ny = y;
						while (--ny >= 0 && up > lup.y)
						{
							if (CELL(x, ny) != '1' ||
								(pos.x < left && x - 1 >=  0 && CELL(x-1, ny) != '1') ||	//for gnawed corners
								(pos.x > right && x + 1 < size.x && CELL(x+1, ny) != '1'))
								up -= csize;			//cell alliance expansion
							else break;
						}
					}
					Rect cell (left, up, right - left, down - up);
					if (globb) cell.Draw (globb, Color (200, 0, 124));//!!! deprecated
					(**i).Collis_brd (cell);
				}
	}
#undef CELL
}
//--------------------------------------------------------------------------------------------------
void Activeman::Process_collisions()
{
    for (iterator i = begin(); i != end(); ++i)
        for (iterator j = begin(); j != i; ++j)
        {
            float r = (**i).Get_r() + (**j).Get_r();
            Vector2f disp = (**i).Get_pos() - (**j).Get_pos();
            if (disp.Lensq() < r*r)
                (**i).Collis_obj (*j);
        }
}
//--------------------------------------------------------------------------------------------------
bool Activeman::Ok() const
{
	for (const_iterator i = begin(); i != end(); ++i)
		if (*i == 0 || !(**i).Ok()) return false;
	return true;
}
//--------------------------------------------------------------------------------------------------