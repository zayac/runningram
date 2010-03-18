/* 
 * File:   TileFactory.h
 * Author: zayac
 *
 * Created on March 19, 2010, 12:10 AM
 */

#ifndef _TILEFACTORY_H
#define	_TILEFACTORY_H

#include "Sprite.h"
#include <map>

enum TileType { GRASS, SAND, WATER };

class Tile {
public:
    virtual void draw(Canvas* buffer, Point point) const = 0;

protected:
    Sprite* _sprite;
};


class MapTile : public Tile {
    bool _obstacle;
    int _friction;
public:
    MapTile (char* fileName, int maxFrames, int animationSpeed, bool obstacle, int friction)
    {
        _sprite = new Sprite(fileName, maxFrames, animationSpeed);
        _obstacle = obstacle;
        _friction = friction;
    }

    MapTile (Canvas* canvas, int maxFrames,int animationSpeed, bool obstacle, int friction)
    {
        _sprite = new Sprite(canvas, maxFrames, animationSpeed);
        _obstacle = obstacle;
        _friction = friction;
    }

    virtual void draw(Canvas* buffer, Point point) const
    {
        _sprite->draw(buffer, point);
    }
};


class TileFactory {
public:
    TileFactory();
    virtual ~TileFactory();

    const Tile& getCharacter( const TileType aKey );

private:
    typedef std::map< TileType, Tile* > mTiles;
    mTiles _tiles;
};

#endif	/* _TILEFACTORY_H */

