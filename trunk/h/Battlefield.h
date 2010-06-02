/* 
 * File:   Battlefield.h
 * Author: necto
 *
 * Created on January 8, 2010, 12:30 AM
 */

#ifndef _BATTLEFIELD_H
#define	_BATTLEFIELD_H

#include <vector>
#include <map>
#include <fstream>
#include "Vec.h"
#include "Graphic_subsystem.h"
#include "IsometricField.h"
#include "TileFactory.h"
#include "Sprite.h"

using std::vector;
using std::multimap;
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

	void cleanField (char fill_cell);
	Point readRespoint (ifstream& file);


    void drawField(Canvas* c) const;
public:

	Battlefield();
	virtual ~Battlefield();

	Serializator* getParser();

	void draw (Graphic_subsystem* c) const;

	const unsigned char* getCells() const {return cells;}
	Point getSize() const {return size;}
	int getCellSize() const {return csize;}
	Point getNextResPoint();

	bool isRough (int x, int y) const;
	bool noRoad (int x, int y) const;
	bool isSand (int x, int y) const;
	float friction (int x, int y) const;

	bool init();
	bool loadFromFile (const char* fname);

	bool ok() const;
};

#endif	/* _BATTLEFIELD_H */

