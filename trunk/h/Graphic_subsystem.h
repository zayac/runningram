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

class Graphic_subsystem
{
    SDL_Surface* screen;
    SDL_Surface* bg;

	class Initialaiser;
	Initialaiser* parser;

public:
	Graphic_subsystem();
	virtual ~Graphic_subsystem();

	Serializator* Get_parser();
	bool Init();
	bool Cleanup();

	void Draw() const;

	bool Ok() const;
};

#endif	/* _GRAPHIC_SUBSYSTEM_H */

