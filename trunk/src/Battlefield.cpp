/* 
 * File:   Battlefield.cpp
 * Author: necto
 * 
 * Created on January 8, 2010, 12:30 AM
 */

#include <fstream>
#include <SDL/SDL.h>
#include <assert.h>
#include "initparser.h"
#include "Battlefield.h"
#include "Graphic_subsystem.h"
#include "TileFactory.h"
#include "Canvas.h"

using std::ifstream;

// <editor-fold defaultstate="collapsed" desc="From file initialaiser">

class Battlefield::Initialaiser : public Sectionp
{
public:
	string filename;
public:

	Initialaiser (string name)
	: Sectionp (name, '='), filename ("field.map")
	{
		addParam (new St_loader<string > ("file", &filename));
	}

	virtual ~Initialaiser ()
	{
		deleteProps ();
	}
}; // </editor-fold>

#define CELL(a, b) cells[size.x*(b) + a]

Battlefield::Battlefield ():cells (0), parser (new Initialaiser("[Map]"))
{
//    _tileFactory.clear();
}
//--------------------------------------------------------------------------------------------------
Battlefield::~Battlefield ()
{
	assert(ok());

	_tileFactory.clear();
	if (cells) delete [] cells;
	delete parser;
}
//--------------------------------------------------------------------------------------------------
Serializator* Battlefield::getParser()
{
	return parser;
}
//--------------------------------------------------------------------------------------------------
void Draw_cage (Canvas* c, Point start, Point full_size, Point num_cells, Color col)
{
	Point size (full_size.x/num_cells.x, full_size.y/num_cells.y);
	for (int i = 0; i <= num_cells.x; ++i)
	{
		Point begin = start + Point (size.x*i, 0);
		Point end = start + Point (size.x*i, full_size.y);
		c->line (begin, end, col);
	}
	for (int i = 0; i <= num_cells.y; ++i)
	{
		Point begin = start + Point (0, size.y*i);
		Point end = start + Point (full_size.x, size.y*i);
		c->line (begin, end, col);
	}
}
//--------------------------------------------------------------------------------------------------
void Battlefield::drawField (Canvas* c) const
{
	int start = 0;
	Rect frame = c->getClipRect ();
        Point tSize = _tileFactory.getSize ();
	frame.enlarge (tSize); //compute manifold of tile positions
	frame.move (-tSize); //really displayed on the screen

	for (int i = 0; i < size.y; i++)
	{
		Point pos = Point (0, i);
		int j = 0;
		while ((pos.x < size.x) && (pos.y >= 0))
		{
			Point coor (start + j * _tileFactory.getSize ().x, i * _tileFactory.getSize ().y / 2);

			if (frame.intersect (Rect(coor - tSize/2, tSize)))//do not process tiles which won't be displayed
				_tileFactory.getTile (CELL (pos.x, pos.y))-> getSprite ()->draw (c, coor);
			pos.x++;
			pos.y--;
			j++;
		}
		start -= _tileFactory.getSize ().x / 2;
	}
	start += _tileFactory.getSize ().x;
	for (int i = 1; i < size.x; i++)
	{
		Point pos = Point (i, size.y - 1);
		int j = size.y;
		while ((pos.x < size.x) && (pos.y >= 0))
		{
			Point coor (start + (size.y - j) * _tileFactory.getSize ().x, (size.y - 1 + i) * _tileFactory.getSize ().y / 2);

			if (frame.contain (coor)) //do not process tiles which won't be displayed
				_tileFactory.getTile (CELL (pos.x, pos.y))->getSprite ()->draw (c, coor);
			pos.x++;
			pos.y--;
			j--;
		}
		start += _tileFactory.getSize ().x / 2;
	}

	for (ziter i = z_buffer.begin (); i != z_buffer.end (); ++i)
	{
		i->second->draw (c);
	}
}
//--------------------------------------------------------------------------------------------------
void Battlefield::draw (Graphic_subsystem* c) const
{
    assert(ok());
    Canvas *canv = c->getScreen ();
    drawField(canv);
}
//--------------------------------------------------------------------------------------------------
void Battlefield::drawMinimap (Graphic_subsystem* c, Rect pos) const
{
    assert(ok());
    Canvas *canv = c->getScreen ();
	Point cellrect = (pos.getSize()|size);

	Point savedPos = canv->getPos();
	canv->setPos(-pos.getLUp());

	for (int i = 0; i < size.x; ++i)
        for (int j = 0; j < size.y; ++j)
        {
			Color bkg =  Color (80, 80, 80);
			if (isRough (i, j)) bkg = Color (100, 50, 50);
			if (isSand (i, j))  bkg = Color (160, 160, 50);

            Draw_cage (canv, Point(i, j)&cellrect, cellrect,
                            Point (CELL(i, j) - '0' + 1, CELL(i, j) - '0' + 1), bkg);
        }
	canv->setPos(savedPos);
}
//--------------------------------------------------------------------------------------------------
bool Battlefield::init()
{
	return loadFromFile (parser->filename.c_str());
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
bool Battlefield::loadFromFile (const char* fname)
{
	ifstream file (fname);
	if (!file.is_open()) return false;
	if (file.bad()) return false;

	file >>size.x;
	file >>size.y;
	file >>csize;
	    
	cells = new unsigned char[size.x * size.y];
	if (cells == 0) return false;
	cleanField ('0');

	for (int i = 0; i < size.y * size.x; )
	{
		char symb = file.get();
		if (symb == '\n') continue;
		cells[i++] = symb;
		
		if (file.eof()) return false;
	}

	No_spaces_begin (file);
	while (file.peek() == '(')
	{
		resur_points.push_back (readRespoint (file));
		No_spaces_begin (file);
	}
	cur_res_point = resur_points.begin();

	_tileFactory.init(file, Point (csize, csize));
	//_tileFactory.scale (Point (csize, csize));
	_tileFactory.toIsometric();

	file.close();
	return ok();
}
//--------------------------------------------------------------------------------------------------
Point Battlefield::readRespoint (ifstream& file)
{
	No_spaces_begin (file);
	Point ret;
	char buf[512];

	file.getline (buf, 512, ',');
	if (buf[0] == '(') buf[0] = ' ';
	ret.x = atoi (buf)*csize + csize / 2;

	file.getline (buf, 512, ')');
	if (buf[0] == ',') buf[0] = ' ';
	ret.y = atoi (buf)*csize + csize / 2;
 
	return ret;
}
//--------------------------------------------------------------------------------------------------
Point Battlefield::getNextResPoint()
{
	if (resur_points.size() == 0) return Point();

	cur_res_point++;
	if (cur_res_point == resur_points.end())
		cur_res_point = resur_points.begin();
	return *cur_res_point;
}
//--------------------------------------------------------------------------------------------------

bool Battlefield::isRough (int x, int y) const
{

    return  _tileFactory.getTile(CELL(x, y))->isObstacle();
}
//--------------------------------------------------------------------------------------------------
bool Battlefield::noRoad (int x, int y) const
{
	return isRough (x, y) || isSand (x, y);
}
//--------------------------------------------------------------------------------------------------
bool Battlefield::isSand (int x, int y) const
{
	return _tileFactory.getTile(CELL(x, y))->isSand();
}
//--------------------------------------------------------------------------------------------------
float Battlefield::friction (int x, int y) const
{
	return _tileFactory.getTile(CELL(x, y))->getFriction();
}
//--------------------------------------------------------------------------------------------------
void Battlefield::cleanZbuffer()
{
	z_buffer.clear();
}
//--------------------------------------------------------------------------------------------------
void Battlefield::addDrawable (int z, Drawable* d)
{
	z_buffer.insert (make_pair (z, d));
}
//--------------------------------------------------------------------------------------------------
void Battlefield::cleanField (char fill_cell)
{
	assert(ok());
	for (int i = size.x*size.y - 1; i >= 0; --i)
	{
		cells[i] = fill_cell;
	}
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
bool Battlefield::ok() const
{
	return parser != 0 && cells != 0;//sizeof cells = 4!! && (sizeof (cells)/sizeof (char) == size.x*size.y);
}
//--------------------------------------------------------------------------------------------------

#undef CELL
