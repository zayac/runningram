/* 
 * File:   Activeman.cpp
 * Author: necto
 * 
 * Created on January 12, 2010, 12:32 AM
 */

#include "Activeman.h"
#include "Car.h"
#include "Battlefield.h"
#include "initparser.h"
#include "DebugHelper.h"
//
Activeman::Activeman ():Transmitted ('A', false) { }
//
//Activeman::Activeman (const Activeman& orig) { }
//
//Activeman::~Activeman () { }


//--------------------------------------------------------------------------------------------------
void Activeman::activate (float dt)
{
	assert(ok());
	for (iterator i = begin(); i != end(); ++i)
		(**i).actions (dt);
}
//--------------------------------------------------------------------------------------------------
void Activeman::draw (Canvas* c)
{
	assert(ok());
	for (iterator i = begin(); i != end(); ++i)
		(**i).draw (c);
}
//--------------------------------------------------------------------------------------------------
inline bool isNotInLimits (const Point& lup, const Point& rdown, const Point & full_size)
{
	return lup.x > full_size.x || lup.y > full_size.y ||
		   rdown.x < 0		   || rdown.y < 0;
}
//-----------------
inline bool isNotInBox (const int left, const int right, const int up, const int down,
						const Point& box_lup, const Point& box_rdown)
{
	return  right < box_lup.x || left > box_rdown.x ||
			up > box_rdown.y  || down < box_lup.y;
}
//-----------------
inline void forceInLimits (Point* lup, Point* rdown, const Point & full_size)
{
	if (lup->x < 0)					lup->x = 0;
	if (lup->y < 0)					lup->y = 0;
	if (rdown->y > full_size.y)	rdown->y = full_size.y;
	if (rdown->x > full_size.x)	rdown->x = full_size.x;
}
//-----------------
inline Rect expandAllianceHorisontal (const Battlefield* bf, const Point& size, const int csize,
									  const int left_lim, const int right_lim, const int center_y,
									  const Point start, int left, int right, int up, int down)
{
	int nx = start.x;
	const int y = start.y;

	while (++nx < size.x && right <= right_lim) // go right
	{
		if (bf->isRough (nx, y)||
			(center_y < up && y - 1 >=  0 && bf->isRough(nx, y-1)) ||	//for gnawed corners
			(center_y > down && y + 1 < size.y && bf->isRough(nx, y+1)))
			right += csize;			//cell alliance expansion
		else break;
	}
	nx = start.x;
	while (--nx >= 0 && left > left_lim) // go left
	{
		if (bf->isRough (nx, y) && bf->isRough (nx, y)||
			(center_y < up && y - 1 >=  0 && bf->isRough (nx, y-1)) ||	//for gnawed corners
			(center_y > down && y + 1 < size.y && bf->isRough (nx, y+1)))
			left -= csize;			//cell alliance expansion
		else break;
	}
	return Rect (left, up, right - left, down - up);
}
//-----------------
inline Rect expandAllianceVertical (const Battlefield* bf, const Point& size, const int csize,
									  const int up_lim, const int down_lim, const int center_x,
									  const Point start, int left, int right, int up, int down)
{
	int ny = start.y;
	const int x = start.x;
	while (++ny < size.y && down <= down_lim) //go down
	{
		if (bf->isRough (x, ny)||
			(center_x < left && x - 1 >=  0 && bf->isRough (x-1, ny)) ||	//for gnawed corners
			(center_x > right && x + 1 < size.x && bf->isRough (x+1, ny)))
			down += csize;			//cell alliance expansion
		else break;
	}
	ny = start.y;
	while (--ny >= 0 && up > up_lim) //go up
	{
		if (bf->isRough (x, ny) ||
			(center_x < left && x - 1 >=  0 && bf->isRough (x-1, ny)) ||	//for gnawed corners
			(center_x > right && x + 1 < size.x && bf->isRough (x+1, ny)))
			up -= csize;			//cell alliance expansion
		else break;
	}
	return Rect (left, up, right - left, down - up);
}
//-----------------
inline void processRoughWalls (const Battlefield* bf, Active* cur,
							   const Point& lup, const Point& pos, const Point& rdown,
							   const Point cell, int left, int right, int up, int down)
{
	const Point size = bf->getSize();
	const int csize = bf->getCellSize();
	Rect wall;

	if (pos.y < up || pos.y > down)//we need a horisontal full lenght wall
		wall = expandAllianceHorisontal	(bf, size, csize,
										 lup.x, rdown.x, pos.y,
										 cell, left, right, up, down);
	else						   // we need a vertical full lenght wall
		wall = expandAllianceVertical (bf, size, csize,
									   lup.y, lup.y, pos.x,
									   cell, left, right, up, down);

	if (DebugHelper::showDBG)
		DebugHelper::c->fillRect (wall, Color (200, 0, 124));
	cur->collisBrd (wall, bf->friction (cell.x, cell.y));
}
//-----------------
#define FOR_ALL_CELLS_IN(cell, limits) \
		for (cell.x = limits.getLeft(); cell.x < limits.getRight(); ++cell.x)\
			for (cell.y = limits.getUp(); cell.y < limits.getDown(); ++cell.y)
//--------------------------------------------------------------------------------------------------
void Activeman::collisBrd (const Battlefield* bf)
{
	assert(ok());
	assert (bf != 0 && bf->ok());

	const unsigned char* cells = bf->getCells ();
	const Point size = bf->getSize();
	const int csize = bf->getCellSize();

	Point full_size = csize*Point (size.x, size.y);

	for (iterator i = begin(); i != end(); ++i)
	{
		float r = (**i).getR();
		Point pos = (**i).getPos ().to<int>();
		Point lup = pos - Point (r, r);
		Point rdown = pos + Point (r, r);

		if (isNotInLimits (lup, rdown, full_size)) continue;

		forceInLimits (&lup, &rdown, full_size);

		Point lup_limit = lup/csize - Point (1, 1);
		Point rdown_limit = rdown/csize + Point (1, 1);
		forceInLimits (&lup_limit, &rdown_limit, size);

		Rect window(lup_limit, rdown_limit - lup_limit);//check only nearest cells
		Point cell;
		FOR_ALL_CELLS_IN(cell, window)
			if (bf->notJustRoad (cell))
			{
				int left = cell.x*csize;
				int right = left + csize;//cell borders
				int up = cell.y*csize;
				int down = up + csize;

				if (isNotInBox (left, right, up, down, lup, rdown)) continue;

				if (bf->isSand (cell))
					(**i).driveSand (Rect(left, up, right - left, down - up),
									 bf->friction (cell));
				else if (bf->isRough (cell))
					processRoughWalls (bf, *i, lup, pos, rdown, cell,
									   left, right, up, down);
			}
	}
}
#undef FOR_ALL_CELLS_IN
//--------------------------------------------------------------------------------------------------
void Activeman::processCollisions()
{
    for (iterator i = begin(); i != end(); ++i)
        for (iterator j = begin(); j != i; ++j)
        {
            float r = (**i).getR() + (**j).getR();
            Vector2f disp = (**i).getPos() - (**j).getPos();
            if (disp.lenSq() < r*r)
                (**i).collisObj (*j);
        }
}
//--------------------------------------------------------------------------------------------------
bool Activeman::deleteDeadalives()
{
	iterator last;
	bool last_must_be_deleted = false;
	bool deletion_done = false;
	for (iterator i = begin(); i != end(); ++i)
	{
		if (last_must_be_deleted)
		{
			delete *last;
			erase (last);
			last_must_be_deleted = false;
			deletion_done = true;
		}
		if ((**i).dead ())
		{
			last = i;
			last_must_be_deleted = true;
		}
	}
	if (last_must_be_deleted)
	{
		delete *last;
		erase (last);
		deletion_done = true;
	}
	return deletion_done;
}
//--------------------------------------------------------------------------------------------------
int Activeman::exp (char* buffer, int size) const
{
	int offset = 0;
	for (const_iterator i = begin(); i != end(); ++i)
	{
		*(buffer + offset++) = 'c';//continue

		*((int*)(buffer + offset)) = (**i).id();
		offset += sizeof (int);
		
		int cur_offs = (**i).expo (buffer + offset, size - offset);
		if (cur_offs == -1) return -1;
		offset += cur_offs;
		
		if (offset > size) return -1;
	}
	*(buffer + offset++) = 's';//stop
	return offset;
}
//--------------------------------------------------------------------------------------------------
int Activeman::imp (char* buffer, int size)
{
    int offset = 0;
    for (iterator i = begin (); /* i != end()*/;)
    {
	char next = *(buffer + offset++);
	if (next == 's') //stop
	{
	    kill (i, end ());
	    return offset;
	}
	if (next != 'c') return -1; //if not 's' and not 'c' I can't determine what it is

	int id = *((int*) (buffer + offset));
	offset += sizeof (int);

	int len = -1;

	if (i == end () || id != (**i).id ())
	{
	    bool found = false;
	    iterator j = i;
	    for (j = i; j != end (); ++j)
		if ((**j).id () == id)
		{
		    found = true;
		    break;
		}
	    if (found)
	    {
		std::swap (*i, *j);
		len = (**i).impo (buffer + offset, size - offset);
	    }
	    //			else
	    //			{
	    //				j = insert (i, new Player ("no name", 0, Key_storage ()));
	    //				len = (**j).Import (buffer + offset, size - offset);
	    //			}
	}
	else len = (**i).impo (buffer + offset, size - offset);

	if (len == -1) return -1;
	offset += len;

	if (i != end ()) ++i;
    }
    return offset;

    //	int offset = 0;
    //	for (const_iterator i = begin(); i != end(); ++i)
    //	{
    //		int cur_offs = (**i).Import (buffer + offset, size - offset);
    //		if (cur_offs == -1) return -1;
    //		offset += cur_offs;
    //		if (*(buffer + cur_offs) != 'e') return -1;//Unknown data
    //		++cur_offs;
    //		if (offset > size) return -1;
    //	}
    //	return offset;
}
//--------------------------------------------------------------------------------------------------
void Activeman::kill (iterator start, iterator finish)
{
	for (iterator i = start; i != finish; ++i)
		(**i).die();
		//delete *i;
//	erase (start, finish);
}
//--------------------------------------------------------------------------------------------------
bool Activeman::ok() const
{
	for (const_iterator i = begin(); i != end(); ++i)
		if (*i == 0 || !(**i).ok()) return false;
	return true;
}
//--------------------------------------------------------------------------------------------------
