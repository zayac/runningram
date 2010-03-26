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
    virtual bool After_read (ifstream &file)
    {
        Sprite * sp = NULL;
    	if (texture_fname.size() > 0)
    	{
            sp = new Sprite (texture_fname.c_str(), 1, 1);
            sp->setPos (Point (sp->getWidth() / 2, sp->getHeight() / 2));
            //_tiles[sym] = sp;
        }

        bool sand = false;
        if ((-aboutnull > (fric - 1) || (fric - 1) > aboutnull) && !rough)
            sand = true;

        _tiles[sym] = new MapTile(sp, rough, fric, sand);
        //cout << "[ " << sym << " ] " << rough << endl;
        texture_fname.clear();
        return true;
    }

    Field_set (string name, MapTile** tiles)
        : Sectionp (name, '='), _tiles(tiles)
    {
        Add_param (new St_loader<unsigned char> ("character", &sym));
	Add_param (new St_loader<string> ("texture", &texture_fname));
	Add_param (new St_loader<int> ("texture centre x", &texture_offset.x));
	Add_param (new St_loader<int> ("texture centre y", &texture_offset.y));
	Add_param (new St_loader<float> ("friction", &fric));
	Add_param (new St_loader<bool> ("rough", &rough));
    }

    virtual ~Field_set ()
    {
	Delete_props ();
    }
};

TileFactory::TileFactory()
{
    
}

void TileFactory::scale(int size)
{
    for( int i = 0; i < 256; i++)
    {
        if ( _tiles[i])
        {
            double rate = (double) size / _tiles[i]->getSprite()->getWidth();
            _tiles[i]->getSprite()->zoom(rate, rate);
        }
    }
}

void TileFactory::init(ifstream& file) {
    clear();
    Sectionp tile_props("gensec", '\n');
    tile_props.Add_param (new Field_set ("tile", _tiles));
    tile_props.Unserialise (file);
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