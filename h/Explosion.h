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


class Explosion
{
    Sprite *spr;
public:
    Explosion();
    Explosion(Sprite*);
    virtual ~Explosion();

    void Draw(Canvas*, int, int, bool *);
};

#endif	/* _EXPLOSION_H */

