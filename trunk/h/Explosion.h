/* 
 * File:   Explosion.h
 * Author: vasily
 *
 * Created on April 7, 2010, 22:15
 */

#ifndef _EXPLOSION_H
#define	_EXPLOSION_H


class Canvas;
class Sprite;
#include "Vec.h"


class Explosion
{
    Sprite *spr;
    Point pos;
    bool run;
    int index;
    int last_anim;
public:
    Explosion (Sprite* spr_, Point pos_);
    virtual ~Explosion();

    void draw (Canvas*);

    bool isRun(void ) const {return run;}
    void animate(void );
};

#endif	/* _EXPLOSION_H */

