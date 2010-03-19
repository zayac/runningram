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
	for (int i = 0; i < 256; ++i)
		sprites[i] = 0;
}
//--------------------------------------------------------------------------------------------------
Battlefield::~Battlefield ()
{
	assert(Ok());

	Clean_sprites();
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
//----------------------------------------
void Battlefield::Draw (Graphic_subsystem* c) const
{
    assert(Ok());
	Canvas *canv = c->Get_screen ();

    for (int i = 0; i < size.x; ++i)
        for (int j = 0; j < size.y; ++j)
        {
			Color bkg =  Color (80, 80, 80);
			if (Is_rough (i, j)) bkg = Color (100, 50, 50);
			if (Is_sand (i, j))  bkg = Color (100, 100, 50);

            Draw_cage (canv, Point(i, j)*csize, Point (csize, csize),
                            Point (CELL(i, j) - '0' + 1, CELL(i, j) - '0' + 1), bkg);
        }
	sprites['1']->draw (canv, Point(120, 120));
}
//--------------------------------------------------------------------------------------------------
bool Battlefield::Init()
{
	return Load_from_file (parser->filename.c_str());
}
//--------------------------------------------------------------------------------------------------
class Field_set : public Sectionp
{
public:
	string texture_fname;
	Point texture_offset;
	unsigned char sym;
	float fric;
	bool rough;

	Sprite** sprites;
	float* frics;
	vector<unsigned char> *roughs;
	vector<unsigned char> *sands;

public:
	virtual bool After_read (ifstream &file)
	{
		if (texture_fname.size() > 0)
		{
			Sprite *sp = new Sprite (texture_fname.c_str(), 1, 1);
			sp->setPos (-texture_offset);
			sprites[sym] = sp;
		}
		if (rough)
			roughs->push_back (sym);

		if ((-aboutnull > (fric - 1) || (fric - 1) > aboutnull) && !rough)
			sands->push_back (sym);

		frics[sym] = fric;
		if ((-aboutnull < (fric - 1) && (fric - 1) < aboutnull) && rough)
			frics[sym] = 0;//if rough then friction about null

		texture_fname.clear();
		fric = 1.0;
		rough = false;
		return true;
	}

	Field_set (string name, Sprite** sps, float* frics_, vector<unsigned char> *roughs_, vector<unsigned char> *sands_)
	:Sectionp (name, '='), sprites(sps), frics(frics_), roughs(roughs_), sands(sands_), texture_fname(), sym(0),
	fric(1.0), rough(false)
	{
		Add_param (new St_loader<unsigned char> ("character", &sym));
		Add_param (new St_loader<string> ("texture", &texture_fname));
		Add_param (new St_loader<int> ("texture centre x", &texture_offset.x));
		Add_param (new St_loader<int> ("texture centre y", &texture_offset.y));
		Add_param (new St_loader<float> ("friction", &fric));
		Add_param (new St_loader<bool> ("rough", &rough));
	}

	virtual ~Field_set ()
	{
		Delete_props ();
	}
};
//--------------------------------------------------------------------------------------------------

bool Battlefield::Load_from_file (const char* fname)
{
	ifstream file (fname);
	if (!file.is_open()) return false;
	if (file.bad()) return false;

	file >>size.x;
	file >>size.y;
	file >>csize;

	cells = new unsigned char[size.x*size.y];
	if (cells == 0) return false;
	Clean_field ('0');

	for (int i = 0; i < size.y*size.x; )
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

	Clean_sprites ();
	Sectionp tile_props("gensec", '\n');
	tile_props.Add_param (new Field_set ("tile", sprites, frics, &roughs, &sands));
	tile_props.Unserialise (file);

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
	ret.x = atoi (buf)*csize + csize/2;

	file.getline (buf, 512, ')');
	if (buf[0] == ',') buf[0] = ' ';
	ret.y = atoi (buf)*csize + csize/2;

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
inline bool contain (const vector<unsigned char>& vec, unsigned char c)
{
	for (vector<unsigned char>::const_iterator i = vec.begin(); i != vec.end(); ++i)
		if (*i == c)return true;
	return false;
}
//--------------------------------------------------------------------------------------------------
bool Battlefield::Is_rough (int x, int y) const
{
	return contain (roughs, CELL(x, y));
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
	return contain (sands, CELL(x, y));
//	return CELL(x, y) == '2' || CELL(x, y) == '3';
}
//--------------------------------------------------------------------------------------------------
float Battlefield::Friction (int x, int y) const
{
	return frics[CELL(x, y)];
//	if (CELL(x, y) == '2')
//		return 5;
//	else if (CELL(x, y) = '3')
//		return -0.2;
//	else
//		return 1;
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
void Battlefield::Clean_sprites()
{
	for (int i = 0; i < 256; ++i)
		if (sprites[i] != 0)
		{
			delete sprites[i];
			sprites[i] = 0;
		}
}
//--------------------------------------------------------------------------------------------------
bool Battlefield::Ok() const
{
	return parser != 0 && cells != 0;//sizeof cells = 4!! && (sizeof (cells)/sizeof (char) == size.x*size.y);
}
//--------------------------------------------------------------------------------------------------

#undef CELL
