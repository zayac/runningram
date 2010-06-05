/* 
 * File:   TileFactory.cpp
 * Author: zayac
 * 
 * Created on March 19, 2010, 12:10 AM
 */

#include "TileFactory.h"
#include "initparser.h"
#include <fstream>
#include <string>
#include "Vec.h"

using namespace std;
class Serializator;

class Field_set : public Sectionp
{
public:
    string texture_fname;
    Point texture_offset;
    unsigned char sym;
    float fric;
    bool rough;
    MapTile** _tiles;

public:
    virtual bool afterRead (ifstream &file)
    {
        Sprite * sp = NULL;
    	if (texture_fname.size() > 0)
    	{
            sp = new Sprite (texture_fname.c_str(), 1, 1);
        }


        bool sand = false;
        if ((-aboutnull > (fric - 1) || (fric - 1) > aboutnull) && !rough)
            sand = true;

        _tiles[sym] = new MapTile(sp, rough, fric, sand);
		//cout << "[ " << sym << " ] " << rough << endl;
        texture_fname.clear();
        return true;
    }

    Field_set (string name, MapTile** tiles) : Sectionp (name, '='), _tiles(tiles)
	{
		addParam (new St_loader<unsigned char> ("character", &sym));
		addParam (new St_loader<string> ("texture", &texture_fname));
		addParam (new St_loader<int> ("texture centre x", &texture_offset.x));
		addParam (new St_loader<int> ("texture centre y", &texture_offset.y));
		addParam (new St_loader<float> ("friction", &fric));
		addParam (new St_loader<bool> ("rough", &rough));
    }

	virtual ~Field_set ()
    {
	deleteProps ();
    }
};

TileFactory::TileFactory()
{
    for (int i = 0; i < 256; ++i)
        if (_tiles[i] != 0)
		{
            _tiles[i] = 0;
		}
}

void TileFactory::scale(Point size)
{
	_size = size;
    for( int i = 0; i < 256; i++)
    {
        if ( _tiles[i])
        {
            double ratex = (double) size.x / _tiles[i]->getSprite()->getWidth();
			double ratey = (double) size.y/ _tiles[i]->getSprite()->getHeight();
            _tiles[i]->getSprite()->zoom(ratex, ratey);
        }
    }
}

void TileFactory::toIsometric()
{
	_size.x = _size.x * 4 - 2;
	_size.y = _size.y * 2;
	for( int i = 0; i < 256; i++)
    {
		if ( _tiles[i])
        {
			_tiles[i]->getSprite()->ortogonalToIsometric();
		}
    }
}

void TileFactory::init(ifstream& file, Point size) 
{
	_size = size;
	clear();
	Sectionp tile_props("gensec", '\n');
	tile_props.addParam (new Field_set ("tile", _tiles));
	tile_props.unserialise (file);
}


TileFactory::~TileFactory() {
}

void TileFactory::clear()
{
    for (int i = 0; i < 256; ++i)
        if (_tiles[i] != 0)
		{
            delete _tiles[i];
            _tiles[i] = 0;
		}	
}
