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
#include <SDL/SDL.h>

Explosion::Explosion()
{
    run = true;
    index = 0;
    last_anim = SDL_GetTicks ();
}

Explosion::~Explosion()
{
//    delete spr;
}

bool Explosion::draw(Canvas* c)
{
    if (run)
    {
        spr->setFrame(index);
        spr->draw(c, pos);
        c->update();
    }
}

void Explosion::setPos(Point pos_)
{
    pos = pos_;
}

void Explosion::setSprite(Sprite* spr_)
{
    spr = spr_;
}

bool Explosion::getRun(void )
{
    return run;
}

void Explosion::animate(void )
{
    static int MaxFrames, speed;

    MaxFrames = spr->getMaxFrames();
    speed = spr->getSpeed();
    
    if(index == (MaxFrames - 1))
    {
        run = false;
        index = 0;
    }
    if(run)
    {
        if (SDL_GetTicks () - last_anim > speed)
        {
            index++;
            last_anim = SDL_GetTicks ();
        }
    }
}