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
}
//--------------------------------------------------------------------------------------------------
Battlefield::~Battlefield ()
{
	assert(Ok());

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
		c->Line (begin, end, col);
	}
	for (int i = 0; i <= num_cells.y; ++i)
	{
		Point begin = start + Point (0, size.y*i);
		Point end = start + Point (full_size.x, size.y*i);
		c->Line (begin, end, col);
	}
}
//----------------------------------------
void Battlefield::Draw (Graphic_subsystem* c) const
{
    assert(Ok());
    
    for (int i = 0; i < size.x; ++i)
        for (int j = 0; j < size.y; ++j)
        {
            Draw_cage (c->Get_screen(), Point(i, j)*csize, Point (csize, csize),
                                Point (CELL(i, j) - '0' + 1, CELL(i, j) - '0' + 1), Color (80, 80, 80));
//            ground_texture[CELL(i, j) - 48]->draw(c->Get_screen(), Point (i * csize, j * csize), csize, csize);
        }

//    ground_texture[8]->draw(c->Get_screen(), Point(40, 40));
//    ground_texture[8]->animate();
   // Draw_cage (c->Get_screen (), Point(), csize*size, size, Color (150, 150, 150));
}
//--------------------------------------------------------------------------------------------------
bool Battlefield::Init()
{
	return Load_from_file (parser->filename.c_str());
}
//--------------------------------------------------------------------------------------------------
bool Battlefield::Load_from_file (const char* fname)
{
	ifstream file (fname);
	if (!file.is_open()) return false;
	if (file.bad()) return false;

	file >>size.x;
	file >>size.y;
	file >>csize;

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

        /* Texture Loader */
        string texture_name;
        //file >> texture_name;
        int n;
        file >> n;

        for(int i = 0; i < n; i++)
        {
            file >> texture_name;
            this->ground_texture.push_back(new Sprite(SDL_LoadBMP(texture_name.c_str()), 1, 1));
        }

	file.close();


        Sprite* spr = new Sprite(SDL_LoadBMP("textures/tommy.bmp"), 13, 50);
        spr->rotate90();
        spr->setTransparency(97,68,43);
        this->ground_texture.push_back(spr);

	return Ok();
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
bool Battlefield::Ok() const
{
	return parser != 0 && cells != 0;//sizeof cells = 4!! && (sizeof (cells)/sizeof (char) == size.x*size.y);
}
//--------------------------------------------------------------------------------------------------

#undef CELL