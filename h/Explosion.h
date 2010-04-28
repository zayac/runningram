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
    Explosion();
    virtual ~Explosion();

    bool Draw(Canvas*);
    void Set_position(Point );
    void Set_sprite(Sprite* );

    bool Get_run(void );
    void Animate(void );
};

#endif	/* _EXPLOSION_H */

