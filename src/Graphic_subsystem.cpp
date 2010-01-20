/* 
 * File:   Graphics_subsistem.cpp
 * Author: necto
 * 
 * Created on January 7, 2010, 8:56 PM
 */

#include <SDL/SDL.h>
#include "Graphic_subsystem.h"
#include "initparser.h"
#include "Camera.h"
#include "Canvas.h"


// <editor-fold defaultstate="collapsed" desc="From file initialaiser">

class Graphic_subsystem::Initialaiser : public Sectionp
{
public:
	int win_x;
	int win_y;
	string win_name;
public:

	Initialaiser (char* name)
	: Sectionp (name, '='), win_x (640), win_y (480), win_name ("Banzay!")
	{
		Add_param (new St_loader<int> ("X size", &win_x));
		Add_param (new St_loader<int> ("Y size", &win_y));
		Add_param (new St_loader<string > ("app name", &win_name));
	}

	virtual ~Initialaiser ()
	{
		Delete_props ();
	}
}; // </editor-fold>


Graphic_subsystem::Graphic_subsystem () :parser (new Initialaiser ((char*)"[Graphics]"))
{
}
//--------------------------------------------------------------------------------------------------
Graphic_subsystem::~Graphic_subsystem ()
{
	Cleanup();//!!! It may be need to add if(initialaised) check for avoid fail of cleanup
	SDL_FreeSurface (buffer);
	buffer = 0;
	delete parser;
}
//--------------------------------------------------------------------------------------------------
Serializator* Graphic_subsystem::Get_parser()
{
	return parser;
}
//--------------------------------------------------------------------------------------------------
bool Graphic_subsystem::Init()
{
	/* initialize SDL */
    SDL_Init (SDL_INIT_VIDEO);

    /* set the title bar */
    SDL_WM_SetCaption (parser->win_name.c_str (), "name");

    /* create window */
    screen = (Canvas*)SDL_SetVideoMode (parser->win_x, parser->win_y, 0, 0);
	buffer = screen->Create_compatible();
//	SDL_LockSurface (screen);

	return Ok();
}
//--------------------------------------------------------------------------------------------------
void Graphic_subsystem::Draw (Camera* look) const
{
    /* update the screen */

	screen->Copy (buffer);
//	SDL_UnlockSurface (screen);
	screen->Update ();
	buffer->Fill (Color(0, 0,0));
//	SDL_LockSurface (screen);
}
//--------------------------------------------------------------------------------------------------
void Graphic_subsystem::Cleanup()
{
    /* cleanup SDL */
    SDL_Quit ();
}
//--------------------------------------------------------------------------------------------------
bool Graphic_subsystem::Ok() const
{
	return parser != 0 && screen != 0 && screen->Ok() && buffer != 0 && buffer->Ok();
}
//--------------------------------------------------------------------------------------------------