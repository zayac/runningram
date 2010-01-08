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


class Graphic_subsystem::Initialaiser :public Sectionp
{
public:
    int win_x;
    int win_y;
    string win_name;
public:
	Initialaiser (string name)
		:Sectionp (name, '='), win_x(640), win_y (480), win_name ("Banzay!")
	{
		Add_param (new St_loader<int> ("X size", &win_x));
		Add_param (new St_loader<int> ("Y size", &win_y));
		Add_param (new St_loader<string > ("app name", &win_name));
	}
	virtual ~Initialaiser()
	{
	    Delete_props();
	}
};

Graphic_subsystem::Graphic_subsystem () :parser (new Initialaiser ("[Graphics]"))
{
}
//--------------------------------------------------------------------------------------------------
Graphic_subsystem::~Graphic_subsystem ()
{
	Cleanup();//!!! It may be need to add if(initialaised) check for avoid fail of cleanup
}
//--------------------------------------------------------------------------------------------------
Serializator* Graphic_subsystem::Get_parser()
{
	if (!parser) parser = new Initialaiser ("[Graphics]");
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
    screen = SDL_SetVideoMode (parser->win_x, parser->win_y, 0, 0);

    /* load bitmap to temp surface */
    SDL_Surface* temp = SDL_LoadBMP ("sdl_logo.bmp");

    /* convert bitmap to display format */
    bg = SDL_DisplayFormat (temp);

    /* free the temp surface */
    SDL_FreeSurface (temp);

	return Ok();
}
//--------------------------------------------------------------------------------------------------
void Graphic_subsystem::Draw (Camera* look) const
{
    /* draw the background */
    SDL_BlitSurface (bg, NULL, screen, NULL);
	
	SDL_Rect simp;
	simp.x = look->Get_pos ().x;
	simp.y = look->Get_pos ().y;
	simp.h = 24;
	simp.w = 100;
	SDL_FillRect (screen, &simp, 0);

    /* update the screen */
    SDL_UpdateRect (screen, 0, 0, 0, 0);
}
//--------------------------------------------------------------------------------------------------
bool Graphic_subsystem::Cleanup()
{
	if (parser)
	{
		delete parser;
		parser = 0;
	}
    /* free the background surface */
    if (bg) SDL_FreeSurface (bg);

    /* cleanup SDL */
    SDL_Quit ();
	return Ok();
}
//--------------------------------------------------------------------------------------------------
bool Graphic_subsystem::Ok() const
{
	return true;
}
//--------------------------------------------------------------------------------------------------