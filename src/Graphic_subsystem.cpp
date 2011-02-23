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
    string splash_file;
	string win_scr_file;
public:

	Initialaiser (char* name)
	: Sectionp (name, '='), win_x (640), win_y (480), win_name ("Banzay!")
        , splash_file ("textures/bricks.png"), win_scr_file("textures/bricks.png")
	{
		addParam (new St_loader<int> ("X size", &win_x));
		addParam (new St_loader<int> ("Y size", &win_y));
		addParam (new St_loader<string> ("app name", &win_name));
        addParam (new St_loader<string> ("splash file", &splash_file));
        addParam (new St_loader<string> ("win screen file", &win_scr_file));
	}

	virtual ~Initialaiser ()
	{
		deleteProps ();
	}
}; // </editor-fold>


Graphic_subsystem::Graphic_subsystem () :parser (new Initialaiser ((char*)"[Graphics]"))
{
}
//--------------------------------------------------------------------------------------------------
Graphic_subsystem::~Graphic_subsystem ()
{
	cleanup();//!!! It may be need to add if(initialaised) check for avoid fail of cleanup
//	SDL_FreeSurface (buffer);
//	buffer = 0;
	delete parser;
}
//--------------------------------------------------------------------------------------------------
Serializator* Graphic_subsystem::getParser()
{
	return parser;
}
//--------------------------------------------------------------------------------------------------
bool Graphic_subsystem::init()
{
	/* initialize SDL */
    SDL_Init (SDL_INIT_VIDEO);

    /* set the title bar */
    SDL_WM_SetCaption (parser->win_name.c_str (), "name");
    
    /* create window */
    screen = Canvas::getScreenCanvas (Point (parser->win_x, parser->win_y));
    buffer = screen->createCompatible();

    /* set the splash picture */
    splash = Canvas(parser->splash_file.c_str());
	win_scr = Canvas(parser->win_scr_file.c_str());

    /*buffer->format->Amask = 0xff000000;
    buffer->format->Ashift = 24;
    buffer->format->Aloss = 0;*/
//	SDL_LockSurface (screen);

	return ok();
}
//--------------------------------------------------------------------------------------------------
void Graphic_subsystem::draw (Camera* look)
{
    /* update the screen */

	buffer.setPos (Point());
	screen->copy (buffer);
//	SDL_UnlockSurface (screen);
	screen->update ();
	buffer.fill (Color(0, 0,0));
	buffer.setPos (look->getPos ());
//	SDL_LockSurface (screen);
}
//--------------------------------------------------------------------------------------------------
void Graphic_subsystem::cleanup()
{
    /* cleanup SDL */
	delete screen;
	screen = 0;
    SDL_Quit ();
}
//--------------------------------------------------------------------------------------------------
bool Graphic_subsystem::ok() const
{
	return parser != 0 && screen != 0 && screen->ok() && buffer.ok();
}
//--------------------------------------------------------------------------------------------------
void Graphic_subsystem::splashScreen()
{
    splash.draw (*screen, Point());
    screen->update();
}
//--------------------------------------------------------------------------------------------------
void Graphic_subsystem::winScreen()
{
    win_scr.draw (*screen, Point());
    screen->update();
}
//--------------------------------------------------------------------------------------------------
