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
		Add_param (new St_loader<string > ("file", &filename));
	}

	virtual ~Initialaiser ()
	{
		Delete_props ();
	}
}; // </editor-fold>

#define CELL(a, b) cells[size.x*(b) + a]

Battlefield::Battlefield ():cells (0), parser (new Initialaiser("[Map]"))
{
    _tileFactory.clear();
}
//--------------------------------------------------------------------------------------------------
Battlefield::~Battlefield ()
{
	assert(Ok());

	_tileFactory.clear();
	if (cells) delete [] cells;
	delete parser;
}
//--------------------------------------------------------------------------------------------------
Serializator* Battlefield::Get_parser()
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

void Battlefield::drawField (Canvas* c) const
{
    int start = 0;
    for(int i = 0; i < size.y; i++)
    {
        Point pos = Point (0, i);
        int j = 0;
        while (( pos.x < size.x) && (pos.y < size.y) && (pos.x >= 0) && (pos.y >= 0))
        {
            //cout << "pos: " << pos.x << ", "<< pos.y << "map: "<< start + j * csize.x << ", " << i * csize.y / 2 << endl;
            _tileFactory.getTile(CELL(pos.x, pos.y))->getSprite()->draw(c, Point (start + j* csize.x, i * csize.y / 2));
            pos.x++;
            pos.y--;
            j++;
        }
        start -= csize.x / 2;
    }

}
//----------------------------------------
void Battlefield::Draw (Graphic_subsystem* c) const
{
    assert(Ok());
    Canvas *canv = c->Get_screen ();

    drawField(canv);
    /*for (int i = 0; i < size.x; ++i)
        for (int j = 0; j < size.y; ++j)
        {
			Color bkg =  Color (80, 80, 80);
			if (Is_rough (i, j)) bkg = Color (100, 50, 50);
			if (Is_sand (i, j))  bkg = Color (100, 100, 50);

            Draw_cage (canv, Point(i, j)*csize, Point (csize, csize),
                            Point (CELL(i, j) - '0' + 1, CELL(i, j) - '0' + 1), bkg);
        }*/
        //test[0]->draw(canv, Point (100, 100));
	//sprites['1']->draw (canv, Point(120, 120));
}
//--------------------------------------------------------------------------------------------------
bool Battlefield::Init()
{
	return Load_from_file (parser->filename.c_str());
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
bool Battlefield::Load_from_file (const char* fname)
{
	ifstream file (fname);
	if (!file.is_open()) return false;
	if (file.bad()) return false;

	file >>size.x;
	file >>size.y;
	file >>csize.x;
    file >>csize.y;
        //test.push_back( new Canvas ("textures/smile.png", true));

	cells = new unsigned char[size.x * size.y];
	if (cells == 0) return false;
	Clean_field ('0');

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
		resur_points.push_back (Read_respoint (file));
		No_spaces_begin (file);
	}
	cur_res_point = resur_points.begin();

	_tileFactory.init(file);
	_tileFactory.scale(csize);
	_tileFactory.toIsometric();
	//Point test = Canvas::transformPointToOrtogonal (Point (2, 3), Point (4, 4));
	csize.x = csize.x * 4 - 2;
	csize.y = csize.y * 2;
	 /*Clean_sprites ();
	Sectionp tile_props("gensec", '\n');
	tile_props.Add_param (new Field_set ("tile", sprites, frics, &roughs, &sands));
	tile_props.Unserialise (file);*/

	file.close();
	return Ok();
}
//--------------------------------------------------------------------------------------------------
Point Battlefield::Read_respoint (ifstream& file)
{
	No_spaces_begin (file);
	Point ret;
	char buf[512];

	file.getline (buf, 512, ',');
	if (buf[0] == '(') buf[0] = ' ';
	ret.x = atoi (buf)*csize.x + csize.y / 2;

	file.getline (buf, 512, ')');
	if (buf[0] == ',') buf[0] = ' ';
	ret.y = atoi (buf)*csize.x + csize.y / 2;
 
	return ret;
}
//--------------------------------------------------------------------------------------------------
Point Battlefield::Get_next_res_point()
{
	if (resur_points.size() == 0) return Point();

	cur_res_point++;
	if (cur_res_point == resur_points.end())
		cur_res_point = resur_points.begin();
	return *cur_res_point;
}
//--------------------------------------------------------------------------------------------------

bool Battlefield::Is_rough (int x, int y) const
{

    return  _tileFactory.getTile(CELL(x, y))->isObstacle();
//	return CELL(x, y) != '1' && CELL(x, y) != '2' && CELL(x, y) != '3';
}
//--------------------------------------------------------------------------------------------------
bool Battlefield::No_road (int x, int y) const
{
	return Is_rough (x, y) || Is_sand (x, y);
//	return CELL(x, y) != '1';
}
//--------------------------------------------------------------------------------------------------
bool Battlefield::Is_sand (int x, int y) const
{
	return _tileFactory.getTile(CELL(x, y))->isSand();
}
//--------------------------------------------------------------------------------------------------
float Battlefield::Friction (int x, int y) const
{
	return _tileFactory.getTile(CELL(x, y))->getFriction();
}
//--------------------------------------------------------------------------------------------------
void Battlefield::Clean_field (char fill_cell)
{
	assert(Ok());
	for (int i = size.x*size.y - 1; i >= 0; --i)
	{
		cells[i] = fill_cell;
	}
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
bool Battlefield::Ok() const
{
	return parser != 0 && cells != 0;//sizeof cells = 4!! && (sizeof (cells)/sizeof (char) == size.x*size.y);
}
//--------------------------------------------------------------------------------------------------

#undef CELL
