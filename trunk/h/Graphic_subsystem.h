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
class Canvas;

class Graphic_subsystem
{
    Canvas* screen;
	Canvas* buffer;

	class Initialaiser;
	Initialaiser* parser;

public:
	Graphic_subsystem();
	virtual ~Graphic_subsystem();

	Serializator* Get_parser();
	bool Init();
	void Cleanup();

	void Draw (Camera* look) const;
	Canvas* Get_screen() const {return buffer;};

	bool Ok() const;
};

#endif	/* _GRAPHIC_SUBSYSTEM_H */

