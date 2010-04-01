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
	class Initialaiser;
	Initialaiser* parser;

public:
	Graphic_subsystem();
	virtual ~Graphic_subsystem();

	Serializator* Get_parser();
	bool Init();
	void Cleanup();
        void SplashScreen();
        void Draw (Camera* look);
	Canvas* Get_screen() {return &buffer;};
        
	bool Ok() const;
};

#endif	/* _GRAPHIC_SUBSYSTEM_H */

