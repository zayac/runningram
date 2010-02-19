/* 
 * File:   Battlefield.h
 * Author: necto
 *
 * Created on January 8, 2010, 12:30 AM
 */

#ifndef _BATTLEFIELD_H
#define	_BATTLEFIELD_H

#include <vector>
#include <fstream>
#include "Vec.h"
#include "Graphic_subsystem.h"
#include "IsometricField.h"
#include "Sprite.h"

using std::vector;
using std::ifstream;
class Serializator;
class SDL_Surface;


class Battlefield
{
	char* cells;
	Point size;
	int csize;
	vector<Point> resur_points;
	vector<Point>::iterator cur_res_point;

        /* Vector for keeping ground textures */
	vector<Sprite*> sprites;
	class Initialaiser;
	Initialaiser* parser;
        IsometricField* test;

	void Clean_field (char fill_cell);
	Point Read_respoint (ifstream& file);

public:
	Battlefield();
	virtual ~Battlefield();

	Serializator* Get_parser();

	void Draw (Graphic_subsystem* c) const;

	const char* Get_cells() const {return cells;}
	Point Get_size() const {return size;}
	int Get_cell_size() const {return csize;}
	Point Get_next_res_point();

	bool Init();
	bool Load_from_file (const char* fname);

	bool Ok() const;
};

#endif	/* _BATTLEFIELD_H */

