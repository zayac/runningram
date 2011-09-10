/* 
 * File:   Graphics_subsistem.cpp
 * Author: necto
 * 
 * Created on January 7, 2010, 8:56 PM
 */

#include <SDL/SDL.h>
#include "initparser.h"
#include "Graphic_subsystem.h"
#include "Camera.h"


// <editor-fold defaultstate="collapsed" desc="From file Initializer">

class Graphic_subsystem::Initializer : public Sectionp
{
public:
	Point fpsPos;
	Color fpsColor;
	int fpsSize;

	Color ftColor;
	Color ftBgColor;
	int ftFontSize;

	Point winSize;
	string win_name;
    string splash_file;
	string win_scr_file;
public:

	Initializer (char* name)
	: Sectionp (name, '='), winSize (640, 480), fpsPos (10, 10),
	  fpsColor (100, 100, 200), fpsSize (16),
	  ftColor (100, 200, 200), ftBgColor (100, 100, 100, 200), ftFontSize (16),
	  win_name ("Banzay!"), splash_file ("textures/bricks.png"),
	  win_scr_file("textures/bricks.png")
	{
		addParam (new St_loader<int> ("X size", &winSize.x));
		addParam (new St_loader<int> ("Y size", &winSize.y));

		addParam (new St_loader<int> ("FPS x", &fpsPos.x));
		addParam (new St_loader<int> ("FPS y", &fpsPos.y));
		addParam (new Color::Initializer ("FPS color", &fpsColor));
		addParam (new St_loader<int> ("FPS font size", &fpsSize));

		addParam (new Color::Initializer ("Frag table font color", &ftColor));
		addParam (new Color::Initializer ("Frag table back ground color", &ftBgColor));
		addParam (new St_loader<int> ("Frag table font size", &ftFontSize));

		addParam (new St_loader<string> ("app name", &win_name));
        addParam (new St_loader<string> ("splash file", &splash_file));
        addParam (new St_loader<string> ("win screen file", &win_scr_file));
	}

	virtual ~Initializer ()
	{
		deleteProps ();
	}
}; // </editor-fold>


Graphic_subsystem::Graphic_subsystem ()
{
}
//--------------------------------------------------------------------------------------------------
Graphic_subsystem::~Graphic_subsystem ()
{
	cleanup();//!!! It may be need to add if(initialaised) check for avoid fail of cleanup
//	SDL_FreeSurface (buffer);
//	buffer = 0;
}
//--------------------------------------------------------------------------------------------------
Serializator* Graphic_subsystem::newParser()
{
	parser = new Initializer ((char*)"[Graphics]");
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
    screen = Canvas::getScreenCanvas (parser->winSize);
    buffer = screen->createCompatible();

    /* set the splash picture */
    splash = Canvas(parser->splash_file.c_str());
	win_scr = Canvas(parser->win_scr_file.c_str());

	fpsFont.openFont ("default.ttf", parser->fpsSize);
	fpsFont.setFG (parser->fpsColor);
	fpsPos = parser->fpsPos;

	ftFont.openFont ("default.ttf", parser->ftFontSize);
	ftFont.setFG (parser->ftColor);
	ftBgColor = parser->ftBgColor;

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
void Graphic_subsystem::drawCompTable (list<Player_manager::Raiting> fragTable)
{
	buffer.pushPos(Point());

	Point size = buffer.getSize();
	Point pos = size/10;

	Canvas c = buffer.createCompatible (size - 2*pos);
	c.setTotalTransparency (ftBgColor.unused);
	c.fill (ftBgColor);

	Rect label_brd;
	label_brd.x = 0;
	label_brd.y = 0;
	label_brd.w = 300;
	label_brd.h = 100;
	for (list<Player_manager::Raiting>::iterator i = fragTable.begin();
			i != fragTable.end(); ++i)
	{
		label_brd.x = 0;

		ftFont.drawLine (&c, i->player.c_str(), &label_brd);
		label_brd.x += 300;

		char buf[256];
		#include <stdlib.h>
		sprintf (buf, "%i", i->frags);
		ftFont.drawLine (&c, buf, &label_brd);
		label_brd.y += ftFont.height();
	}
	c.draw (buffer, pos);
	buffer.popPos();
}
//--------------------------------------------------------------------------------------------------
void Graphic_subsystem::drawFps (float dt)
{
	static float instant_fps = 0;
	static float fps = 0;
	instant_fps = 1/(dt == 0 ? 1: dt);
	fps = (instant_fps + 50*fps)/51;

	Rect brd (buffer.getPos() + fpsPos, Point (100, 100));
	char buf[128];
	sprintf (buf, "FPS: %.2f", fps);
	fpsFont.drawLine (&buffer, buf, &brd);
}
//--------------------------------------------------------------------------------------------------
void Graphic_subsystem::winScreen()
{
    win_scr.draw (*screen, Point());
    screen->update();
}
//--------------------------------------------------------------------------------------------------
