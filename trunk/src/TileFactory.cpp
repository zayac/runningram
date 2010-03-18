/* 
 * File:   TileFactory.cpp
 * Author: zayac
 * 
 * Created on March 19, 2010, 12:10 AM
 */

#include "TileFactory.h"

TileFactory::TileFactory() {
}

TileFactory::~TileFactory() {
}

const Tile& TileFactory::getCharacter( const TileType aKey )
{
    Tile* tile = NULL;
    mTiles::iterator it = _tiles.find( aKey );

    if ( _tiles.end() == it )
    {
        switch (aKey)
        {
            case GRASS:
                tile = new MapTile( "textures/grass.bmp", 1, 10, false, 1);
                break;
            case WATER:
                tile = new MapTile( "textures/water.bmp", 1, 10, false, 1);
                break;
            default:
                tile = new MapTile( "textures/sand.bmp", 1, 10, false, 1);
                break;
        }
        _tiles.insert( make_pair( aKey, tile ) );
    }
    else
    {
        tile = it->second;
    }
    return *tile;
}