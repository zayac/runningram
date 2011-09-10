/* 
 * File:   Graphics_subsistem.h
 * Author: necto
 *
 * Created on January 7, 2010, 8:56 PM
 */

#ifndef _GRAPHIC_SUBSYSTEM_H
#define	_GRAPHIC_SUBSYSTEM_H

#include "Canvas.h"
#include "Fontc.h"

#include "Player_manager.h"

//struct SDL_Surface;
class Serializator;
class Camera;

class Graphic_subsystem
{
	Canvas* screen;
	Canvas buffer;
    Canvas splash;
	Canvas win_scr;
	class Initializer;
	Initializer* parser;

	Fontc fpsFont;
	Point fpsPos;

	Fontc ftFont;
	Color ftBgColor;

public:
	Graphic_subsystem();
	virtual ~Graphic_subsystem();

	Serializator* newParser();
	bool init();
	void cleanup();
    void splashScreen();
    void drawFps (float dt);
    void drawCompTable (list<Player_manager::Raiting> fragTable);
	void winScreen();
    void draw (Camera* look);
	Canvas* getScreen() {return &buffer;};
	Point getCenter() const
		{return buffer.getClipRect().getCenter();}
        
	bool ok() const;
};

#endif	/* _GRAPHIC_SUBSYSTEM_H */

