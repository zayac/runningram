/* 
 * File:   Battlefield.cpp
 * Author: necto
 * 
 * Created on January 8, 2010, 12:30 AM
 */

#include <fstream>
#include <SDL/SDL.h>
#include "initparser.h"
#include "Battlefield.h"
#include "Graphic_subsystem.h"

using std::ifstream;

// <editor-fold defaultstate="collapsed" desc="From file initialaiser">

class Battlefield::Initialaiser : public Sectionp
{
public:
	string filename;
public:

	Initialaiser (string name)
	: Sectionp (name, '='), filename ("default.map")
	{
		Add_param (new St_loader<string > ("file", &filename));
	}

	virtual ~Initialaiser ()
	{
		Delete_props ();
	}
}; // </editor-fold>


#define CELL(x, y) cells[size.x*(y) + x]

Battlefield::Battlefield ():cells (0), parser (new Initialaiser("[Map]"))
{
}
//--------------------------------------------------------------------------------------------------
Battlefield::~Battlefield ()
{
    /* free the background surface */
    if (bg) SDL_FreeSurface (bg);//!!! must be deleted width bg

	if (cells) delete [] cells;
	delete parser;
}
//--------------------------------------------------------------------------------------------------
Serializator* Battlefield::Get_parser()
{
	return parser;
}
//--------------------------------------------------------------------------------------------------
void Battlefield::Draw (Canvas* c) const
{
    /* draw the background */
    SDL_BlitSurface (bg, NULL, c->Get_screen(), NULL);//must be replaced by more smarter content
}
//--------------------------------------------------------------------------------------------------
bool Battlefield::Init()
{
    /* load bitmap to temp surface */
    SDL_Surface* temp = SDL_LoadBMP ("sdl_logo.bmp");

    /* convert bitmap to display format */
    bg = SDL_DisplayFormat (temp);

    /* free the temp surface */
    SDL_FreeSurface (temp);

	//^^^^ must be deleted in future


	return Load_from_file (parser->filename.c_str());
}
//--------------------------------------------------------------------------------------------------
bool Battlefield::Load_from_file (const char* fname)
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