/* 
 * File:   Explosion.cpp
 * Author: vasily
 * 
 * Created on April 7, 2010, 22:15
 */

#include "Explosion.h"
#include "Sprite.h"
#include "Canvas.h"
#include <unistd.h>
#include "Vec.h"

Explosion::Explosion()
{
//    spr = new Sprite("textures/image2_alpha.png", 33, 1, true);
}

Explosion::~Explosion()
{
//    delete spr;
}

bool Explosion::Draw(Canvas* c)
{
    static int max = spr->getMaxFrames();

    
    if (spr->getFrame() == 0) spr->start();
    spr->draw(c, pos);
    c->update();
    spr->animate();
    if (spr->getFrame() == 0)
    {
        spr->stop();
        return true;
    }
    return false;
}

void Explosion::Set_position(Point pos_)
{
    pos = pos_;
}

void Explosion::Set_sprite(Sprite* spr_)
{
    spr = spr_;
}