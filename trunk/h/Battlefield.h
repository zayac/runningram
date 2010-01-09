/* 
 * File:   Battlefield.h
 * Author: necto
 *
 * Created on January 8, 2010, 12:30 AM
 */

#ifndef _BATTLEFIELD_H
#define	_BATTLEFIELD_H

#include "Vec.h"
#include "Graphic_subsystem.h"

class Serializator;

class SDL_Surface;

class Battlefield
{
    SDL_Surface* bg;//deprecated

	char* cells;
	Polong size;

	class Initialaiser;
	Initialaiser* parser;

	void Clean_field (char fill_cell);

public:
	Battlefield();
	virtual ~Battlefield();

	Serializator* Get_parser();

	void Draw (Canvas* c) const;

	bool Init();
	bool Load_from_file (const char* fname);
private:

};

#endif	/* _BATTLEFIELD_H */

