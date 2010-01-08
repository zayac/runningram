/* 
 * File:   Graphics_subsistem.h
 * Author: necto
 *
 * Created on January 7, 2010, 8:56 PM
 */

#ifndef _GRAPHIC_SUBSYSTEM_H
#define	_GRAPHIC_SUBSYSTEM_H

struct SDL_Surface;
class Serializator;
class Camera;

class Graphic_subsystem
{
    SDL_Surface* screen;

	class Initialaiser;
	Initialaiser* parser;

public:
	Graphic_subsystem();
	virtual ~Graphic_subsystem();

	Serializator* Get_parser();
	bool Init();
	bool Cleanup();

	void Draw (Camera* look) const;
	SDL_Surface* Get_screen() const {return screen;};

	bool Ok() const;
};

typedef Graphic_subsystem Canvas;

#endif	/* _GRAPHIC_SUBSYSTEM_H */

