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

Explosion::Explosion (Sprite* spr_, Point pos_)
	:spr(spr_), pos (pos_), index (0), run (true)
{
    last_anim = SDL_GetTicks ();
}

Explosion::~Explosion()
{
//    delete spr;
}

void Explosion::draw (Canvas* c)
{
    if (run)
    {
        spr->setFrame (index);
        spr->draw (c, pos);
    }
}

void Explosion::animate()
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