/* 
 * File:   Graphics_subsistem.h
 * Author: necto
 *
 * Created on January 7, 2010, 8:56 PM
 */

#ifndef _GRAPHIC_SUBSYSTEM_H
#define	_GRAPHIC_SUBSYSTEM_H

#include "Canvas.h"

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

public:
	Graphic_subsystem();
	virtual ~Graphic_subsystem();

	Serializator* getParser();
	bool init();
	void cleanup();
    void splashScreen();
	void winScreen();
    void draw (Camera* look);
	Canvas* getScreen() {return &buffer;};
        
	bool ok() const;
};

#endif	/* _GRAPHIC_SUBSYSTEM_H */

