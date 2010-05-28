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
#include "TileFactory.h"
#include "Sprite.h"

using std::vector;
using std::ifstream;
class Serializator;
class SDL_Surface;


class Battlefield
{
	unsigned char* cells;
	Point size;
	int csize;
	vector<Point> resur_points;
	vector<Point>::iterator cur_res_point;
    TileFactory _tileFactory;

	class Initialaiser;
	Initialaiser* parser;

	void Clean_field (char fill_cell);
	Point Read_respoint (ifstream& file);


    void drawField(Canvas* c) const;
public:

	Battlefield();
	virtual ~Battlefield();

	Serializator* Get_parser();

	void Draw (Graphic_subsystem* c) const;

	const unsigned char* Get_cells() const {return cells;}
	Point Get_size() const {return size;}
	int Get_cell_size() const {return csize;}
	Point Get_next_res_point();

	bool Is_rough (int x, int y) const;
	bool No_road (int x, int y) const;
	bool Is_sand (int x, int y) const;
	float Friction (int x, int y) const;

	bool Init();
	bool Load_from_file (const char* fname);

	bool Ok() const;
};

#endif	/* _BATTLEFIELD_H */

