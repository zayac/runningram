/* 
 * File:   TileFactory.h
 * Author: zayac
 *
 * Created on March 19, 2010, 12:10 AM
 */

#ifndef _TILEFACTORY_H
#define	_TILEFACTORY_H

#include "Sprite.h"

enum TileType { GRASS, SAND, WATER };

class Tile {
public:
    virtual void draw(Canvas* buffer, Point point) const = 0;
    inline virtual Sprite* getSprite() { return _sprite; }
protected:
    Sprite* _sprite;
};


class MapTile : public Tile {
    bool _obstacle;
    bool _sand;
    float _friction;
    unsigned char _symbol;
public:
    inline bool isObstacle() { return _obstacle; }
    inline bool isSand() { return _sand; }
    inline float getFriction() { return _friction; }

    MapTile (char* fileName, int maxFrames, int animationSpeed, bool alpha = false, bool obstacle = false, float friction = 1.0f, bool sand = false)
    {
        _sprite = new Sprite(fileName, maxFrames, animationSpeed, alpha);
        _obstacle = obstacle;
        _friction = friction;
        _sand = sand;
    }

    MapTile (Canvas* canvas, int maxFrames,int animationSpeed, bool obstacle = false, float friction = 1.0f, bool sand = false)
    {
        _sprite = new Sprite(canvas, maxFrames, animationSpeed);
        _obstacle = obstacle;
        _friction = friction;
        _sand = sand;
    }

    MapTile (Sprite* sprite, bool obstacle = false, float friction = 1.0f, bool sand = false)
    {
        _sprite = sprite;
        _obstacle = obstacle;
        _friction = friction;
        _sand = sand;
    }

    virtual void draw(Canvas* buffer, Point point) const
    {
        _sprite->draw(buffer, point);
    }

    ~MapTile ()
    {
        delete _sprite;
    }
};


class TileFactory {
public:
    TileFactory();
    inline MapTile* getTile(char chr) const { return _tiles[chr]; }
    void init (ifstream& file);
    void scale(Point size);
    void toIsometric();
    virtual ~TileFactory();
    void clear();
private:
    MapTile* _tiles[256];

};

#endif	/* _TILEFACTORY_H */

