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
#include "Drawable.h"

using std::vector;
using std::multimap;
using std::ifstream;
class Serializator;
class Interpreter;

const int ScriptFnameMaxLen = 2048;

class Battlefield
{
	unsigned char* cells;
	Point size;
	int csize;
	char script_fname[ScriptFnameMaxLen];
	vector<Point> resur_points;
	vector<Point>::iterator cur_res_point;
    TileFactory _tileFactory;
	multimap<int, Drawable*> z_buffer;
	typedef multimap<int, Drawable*>::const_iterator ziter;

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
	void drawMinimap (Canvas* c, Point limit) const;

	const unsigned char* getCells() const {return cells;}
	Point getSize() const {return size;}
	Point getFullSize() const {return size*csize;}
	int getCellSize() const {return csize;}
	Point getNextResPoint();

	bool isRough (int x, int y) const;
	bool noRoad (int x, int y) const;
	bool isSand (int x, int y) const;
	float friction (int x, int y) const;

	inline bool isRough (Point cell) const
	{return isRough (cell.x, cell.y);}
	inline bool noRoad (Point cell) const
	{return noRoad (cell.x, cell.y);}
	inline bool isSand (Point cell) const
	{return isSand (cell.x, cell.y);}
	inline float friction (Point cell) const
	{return friction (cell.x, cell.y);}

	void cleanZbuffer();
	void addDrawable (int z, Drawable* d);
//	void adsorb (vector<Drawable*>*)

	bool init (Interpreter* interp);
	bool loadFromFile (const char* fname);

	bool ok() const;
};

#endif	/* _BATTLEFIELD_H */

