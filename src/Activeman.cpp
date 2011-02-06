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
Canvas* globb = 0;//!!! deprecated, must be deleted
void Activeman::draw (Canvas* c)
{
	globb = c;

	assert(ok());
	for (iterator i = begin(); i != end(); ++i)
		(**i).draw (c);
}
//--------------------------------------------------------------------------------------------------
#define CELL(a, b) cells[size.x*(b) + a]
//--------------------------------------------------------------------------------------------------
void Activeman::collisBrd (const Battlefield* bf)
{
	assert(ok());
	assert (bf != 0 && bf->ok());

	const unsigned char* cells = bf->getCells ();
	Point size = bf->getSize ();
	int csize = bf->getCellSize();

	Point full_size = Point (csize*size.x, csize*size.y);

	for (iterator i = begin(); i != end(); ++i)
	{
		float r = (**i).getR();
		Point pos = (**i).getPos ().to<int>();
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
				if (bf->noRoad(x, y))
				{
					int left = x*csize;
					int right = left + csize;//cell borders
					int up = y*csize;
					int down = up + csize;

					if (right < lup.x) continue;
					if (left > rdown.x) continue;//necessary conditions
					if (up > rdown.y) continue;
					if (down < lup.y) continue;

					if (bf->isSand (x, y))
					{
						Rect cell (left, up, right - left, down - up);
						(**i).driveSand (cell, bf->friction (x, y));
						continue;						//sands acts lonesome
					}

					if (pos.y < up || pos.y > down)//we need a horisontal full lenght wall
					{
						int nx = x;

						while (++nx < size.x && right <= rdown.x)
						{
							if (bf->isRough (nx, y)||
								(pos.y < up && y - 1 >=  0 && bf->isRough(nx, y-1)) ||	//for gnawed corners
								(pos.y > down && y + 1 < size.y && bf->isRough(nx, y+1)))
								right += csize;			//cell alliance expansion
							else break;
						}
						nx = x;
						while (--nx >= 0 && left > lup.x)
						{
							if (bf->isRough (nx, y) && bf->isRough (nx, y)||
								(pos.y < up && y - 1 >=  0 && bf->isRough (nx, y-1)) ||	//for gnawed corners
								(pos.y > down && y + 1 < size.y && bf->isRough (nx, y+1)))
								left -= csize;			//cell alliance expansion
							else break;
						}
					}
					else // we need a vertical full lenght wall
					{
						int ny = y;
						while (++ny < size.y && down <= rdown.y)
						{
							if (bf->isRough (x, ny)||
								(pos.x < left && x - 1 >=  0 && bf->isRough (x-1, ny)) ||	//for gnawed corners
								(pos.x > right && x + 1 < size.x && bf->isRough (x+1, ny)))
								down += csize;			//cell alliance expansion
							else break;
						}
						ny = y;
						while (--ny >= 0 && up > lup.y)
						{
							if (bf->isRough (x, ny) ||
								(pos.x < left && x - 1 >=  0 && bf->isRough (x-1, ny)) ||	//for gnawed corners
								(pos.x > right && x + 1 < size.x && bf->isRough (x+1, ny)))
								up -= csize;			//cell alliance expansion
							else break;
						}
					}
					Rect cell (left, up, right - left, down - up);
					if (globb) globb->fillRect (cell, Color (200, 0, 124));//!!! deprecated
					(**i).collisBrd (cell, bf->friction (x, y));
				}
	}
}
#undef CELL
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
