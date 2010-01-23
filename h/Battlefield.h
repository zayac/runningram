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
#include <vector>
class Serializator;

class SDL_Surface;

class Battlefield
{
	char* cells;
	Point size;
	int csize;

        /* Vector for keeping ground textures */
	std::vector<SDL_Surface*> ground_texture;
	class Initialaiser;
	Initialaiser* parser;

	void Clean_field (char fill_cell);

public:
	Battlefield();
	virtual ~Battlefield();

	Serializator* Get_parser();

	void Draw (Graphic_subsystem* c) const;

	const char* Get_cells() const {return cells;}
	Point Get_size() const {return size;}
	int Get_cell_size() const {return csize;}

	bool Init();
	bool Load_from_file (const char* fname);

	bool Ok() const;
};

#endif	/* _BATTLEFIELD_H */

